#include <iostream>
#include <string>

using namespace std;

class AdmitCard
{
public:
    virtual string getClass() = 0;
};

class CalTechAdmitCard : public AdmitCard
{
public:
    string getClass() override
    {
        return "CalTechAdmitCard";
    }
};

class MITAdmitCard : public AdmitCard
{
public:
    string getClass() override
    {
        return "MITAdmitCard";
    }
};

class GeorgiaTechAdmitCard : public AdmitCard
{
public:
    string getClass() override
    {
        return "GeorgiaTechAdmitCard";
    }
};

class USCAdmitCard : public AdmitCard
{
public:
    string getClass() override
    {
        return "USCAdmitCard";
    }
};

class VSUAdmitCard : public AdmitCard
{
public:
    string getClass() override
    {
        return "VSUAdmitCard";
    }
};

class ASUAdmitCard : public AdmitCard
{
public:
    string getClass() override
    {
        return "ASUAdmitCard";
    }
};

class FeeCalculator
{
public:
    virtual int calculateFee() = 0;
};

class CalTechFeeCalculator : public FeeCalculator
{
public:
    int calculateFee() override
    {
        return 3000;
    }
};

class MITFeeCalculator : public FeeCalculator
{
public:
    int calculateFee() override
    {
        return 4000;
    }
};

class GeorgiaTechFeeCalculator : public FeeCalculator
{
public:
    int calculateFee() override
    {
        return 7000;
    }
};

class USCFeeCalculator : public FeeCalculator
{
public:
    int calculateFee() override
    {
        return 500;
    }
};

class VSUFeeCalculator : public FeeCalculator
{
public:
    int calculateFee() override
    {
        return 1000;
    }
};

class ASUFeeCalculator : public FeeCalculator
{
public:
    int calculateFee() override
    {
        return 0;
    }
};

class UniversityFactory
{
protected:
    static const int cutoff = 109;

public:
    static UniversityFactory *getUniversityFactory(int score);
    virtual FeeCalculator *getFeeCalculator(string course) = 0;
    virtual AdmitCard *getAdmitcard(string course) = 0;
};

class IvyLeagueUniversityFactory : public UniversityFactory
{
public:
    FeeCalculator *getFeeCalculator(string course) override
    {
        if (course == "Math")
        {
            return new MITFeeCalculator();
        }
        else if (course == "Physics")
        {
            return new CalTechFeeCalculator();
        }
        else if (course == "CS")
        {
            return new GeorgiaTechFeeCalculator();
        }
        else
        {
            return nullptr;
        }
    }

    AdmitCard *getAdmitcard(string course) override
    {
        if (course == "Math")
        {
            return new MITAdmitCard();
        }
        else if (course == "Physics")
        {
            return new CalTechAdmitCard();
        }
        else if (course == "CS")
        {
            return new GeorgiaTechAdmitCard();
        }
        else
        {
            return nullptr;
        }
    }
};

class PublicUniversityFactory : public UniversityFactory
{
public:
    FeeCalculator *getFeeCalculator(string course) override
    {
        if (course == "Math")
        {
            return new USCFeeCalculator();
        }
        else if (course == "Physics")
        {
            return new VSUFeeCalculator();
        }
        else if (course == "CS")
        {
            return new ASUFeeCalculator();
        }
        else
        {
            return nullptr;
        }
    }
    AdmitCard *getAdmitcard(string course) override
    {
        if (course == "Math")
        {
            return new USCAdmitCard();
        }
        else if (course == "Physics")
        {
            return new VSUAdmitCard();
        }
        else if (course == "CS")
        {
            return new ASUAdmitCard();
        }
        else
        {
            return nullptr;
        }
    }
};

UniversityFactory *UniversityFactory::getUniversityFactory(int score)
{
    if (score > cutoff)
        return new IvyLeagueUniversityFactory();
    else
        return new PublicUniversityFactory();
}

int main()
{
    int score = 125;
    UniversityFactory *abstractUniversityFactory = UniversityFactory::getUniversityFactory(score);

    AdmitCard *admitCard = abstractUniversityFactory->getAdmitcard("Math");
    cout << admitCard->getClass() << endl;

    FeeCalculator *feeCalculator = abstractUniversityFactory->getFeeCalculator("Math");
    cout << feeCalculator->calculateFee() << endl;

    // example 2
    score = 99;
    AdmitCard *admitCard2 = abstractUniversityFactory->getAdmitcard("CS");
    cout << admitCard2->getClass() << endl;

    FeeCalculator *feeCalculator2 = abstractUniversityFactory->getFeeCalculator("Math");
    cout << feeCalculator2->calculateFee() << endl;

    return 0;
}