#include <bits/stdc++.h>
#include <ctime>

using namespace std;

// Forward declarations
class Basket;
class Item;
class BasketRule;

// Item class
class Item
{
public:
    std::string name;
    double price;

    Item(const std::string &name, double price) : name(name), price(price) {}

    string getItemDetails() const
    {

        string itemDetails = "Name: " + name + "\nPrice: " + to_string(price) + "\n---";
        return itemDetails;
    }
};

// Basket class
class Basket
{
public:
    std::vector<Item> items;
    double total;
    std::time_t created;
    Basket() : total(0.0), created(std::time(nullptr)) {}

    void addItem(const Item &item)
    {
        items.push_back(item);
        total += item.price;
    }

    void decreaseTotal(double amount)
    {
        total -= amount;
    }

    std::time_t getCreated() const
    {
        return created;
    }

    double getTotal() const
    {
        return total;
    }
    std::string getCurrentTime() const
    {
        std::ostringstream oss;
        oss << "Created: " << std::put_time(std::localtime(&created), "%F %T") << "\n";
        return oss.str();
    }

    std::string getBasketDetails() const
    {
        std::string output = "Basket:\n" + getCurrentTime() + "\n";

        for (const auto &item : items)
        {
            output += item.getItemDetails() + "\n";
        }
        output += "========\nTotal: " + std::to_string(total);
        return output;
    }
};

// BasketRule abstract class
class BasketRule
{
public:
    virtual bool matches(const Basket &basket) const = 0;
    virtual void apply(Basket &basket) const = 0;
    virtual ~BasketRule() = default;
};

// HalloweenPromotion class
class HalloweenPromotion : public BasketRule
{
public:
    bool matches(const Basket &basket) const override
    {
        std::time_t now_time_t = std::time(nullptr);
        std::tm *now_tm = std::localtime(&now_time_t);

        std::time_t created_time_t = basket.getCreated();
        std::tm *created_tm = std::localtime(&created_time_t);

        return (now_tm->tm_mon == 9 && created_tm->tm_mon == 9); // October is month 9 in tm struct
    }

    void apply(Basket &basket) const override
    {
        std::cout << "Applying HalloweenPromotion promotion -13.00\n";
        basket.addItem(Item("Promo - Creepy Skeleton Suit", 0.00));
        basket.decreaseTotal(13.00);
    }
};

// DiscountForBigOrdersPromotion class
class DiscountForBigOrdersPromotion : public BasketRule
{
public:
    bool matches(const Basket &basket) const override
    {
        return basket.getTotal() > 1000.00;
    }

    void apply(Basket &basket) const override
    {
        std::cout << "Applying DiscountForBigOrdersPromotion promotion -100.00\n";
        basket.decreaseTotal(100.00);
    }
};

// ChristmasPromotion class
class ChristmasPromotion : public BasketRule
{
public:
    bool matches(const Basket &basket) const override
    {
        std::time_t now_time_t = std::time(nullptr);
        std::tm *now_tm = std::localtime(&now_time_t);

        std::time_t created_time_t = basket.getCreated();
        std::tm *created_tm = std::localtime(&created_time_t);

        return (now_tm->tm_mon == 11 && created_tm->tm_mon == 11 && created_tm->tm_mday > 20); // December is month 11 in tm struct
    }

    void apply(Basket &basket) const override
    {
        std::cout << "Applying ChristmasPromotion promotion -10.00\n";
        basket.addItem(Item("Promo - Santa Hat", 0.00));
        basket.decreaseTotal(10.00);
    }
};

// PromotionEngine class
class PromotionEngine
{
private:
    std::vector<BasketRule *> promotionRules;

public:
    PromotionEngine(const vector<BasketRule *> &rules) : promotionRules(rules) {}

    void process(Basket &basket)
    {
        std::cout << "=== BEFORE Promotions ===" << std::endl;
        std::cout << basket.getBasketDetails() << std::endl;

        std::cout << "\nApplying promotions" << std::endl;
        for (const auto &rule : promotionRules)
        {
            if (rule->matches(basket))
            {
                rule->apply(basket);
            }
        }
        std::cout << "Promotions applied\n"
                  << std::endl;

        std::cout << "=== AFTER Promotions ===" << std::endl;
        std::cout << basket.getBasketDetails() << std::endl;
    }
};

// Main function
int main()
{
    // Create sample rules
    BasketRule *rule1 = new HalloweenPromotion();
    BasketRule *rule2 = new DiscountForBigOrdersPromotion();
    BasketRule *rule3 = new ChristmasPromotion();

    // Create a vector of promotion rules
    std::vector<BasketRule *> promotionRules = {rule1, rule2, rule3};

    // Create a PromotionEngine with the rules
    PromotionEngine engine(promotionRules);

    // Create a Basket
    Basket basket;
    basket.addItem(Item("Laptop", 1200.00));
    basket.addItem(Item("Phone", 800.00));

    // Process the basket with the promotion engine
    engine.process(basket);

    return 0;
}
