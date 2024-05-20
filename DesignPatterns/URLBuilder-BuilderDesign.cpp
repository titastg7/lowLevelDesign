#include <bits/stdc++.h>

using namespace std;

class URLBuilder
{

public:
    class Builder
    {
    public:
        std::string protocol;
        std::string port;
        std::string hostname;
        std::string pathParam;
        std::string queryParam;
        friend class URLBuilder;

        URLBuilder *build()
        {
            return new URLBuilder(this);
        }
        Builder *setProtocol(const std::string &protocol)
        {
            this->protocol = protocol;
            return this;
        }

        Builder *sethostname(const std::string &hostname)
        {
            this->hostname = hostname;
            return this;
        }

        Builder *setport(const std::string &port)
        {
            this->port = port;
            return this;
        }

        Builder *setpathParam(const std::string &pathParam)
        {
            this->pathParam = pathParam;
            return this;
        }

        Builder *setqueryParam(const std::string &queryParam)
        {
            this->queryParam = queryParam;
            return this;
        }
    };

public:
    string protocol;
    string hostname;
    string port;
    string pathParam;
    string queryParam;

private:
    URLBuilder(Builder *builder)
    {
        protocol = builder->protocol;
        hostname = builder->hostname;
        port = builder->port;
        pathParam = builder->pathParam;
        queryParam = builder->queryParam;
    }
};

int main()
{
    URLBuilder::Builder bou = URLBuilder::Builder();
    bou.setProtocol("https");
    bou.sethostname("newwebsite")->setport("443");
    URLBuilder *builder = bou.build();
    cout << "Protocol: " << builder->protocol << std::endl;
    cout << "Hostname: " << builder->hostname << std::endl;
    cout << "Port: " << builder->port << std::endl;
    return 0;
}
