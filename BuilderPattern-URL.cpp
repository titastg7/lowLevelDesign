#include <bits/stdc++.h>
using namespace std;

class URLBuilder
{
public:
    const std::string protocol;
    const std::string hostname;
    const std::string port;
    const std::string pathParam;
    const std::string queryParam;

private:
    // Private constructor to be called by the Builder
    URLBuilder(const std::string &protocol,
               const std::string &hostname,
               const std::string &port,
               const std::string &pathParam,
               const std::string &queryParam)
        : protocol(protocol),
          hostname(hostname),
          port(port),
          pathParam(pathParam),
          queryParam(queryParam) {}

    // Declare Builder as a friend to allow access to the private constructor
    friend class Builder;
};

class Builder
{
private:
    std::string protocol;
    std::string hostname;
    std::string port;
    std::string pathParam;
    std::string queryParam;

public:
    Builder &setProtocol(const std::string &protocol)
    {
        this->protocol = protocol;
        return *this;
    }

    Builder &setHostname(const std::string &hostname)
    {
        this->hostname = hostname;
        return *this;
    }

    Builder &setPort(const std::string &port)
    {
        this->port = port;
        return *this;
    }

    Builder &setPathParam(const std::string &pathParam)
    {
        this->pathParam = pathParam;
        return *this;
    }

    Builder &setQueryParam(const std::string &queryParam)
    {
        this->queryParam = queryParam;
        return *this;
    }

    URLBuilder build()
    {
        return URLBuilder(protocol, hostname, port, pathParam, queryParam);
    }
};

int main()
{
    URLBuilder url = Builder()
                         .setProtocol("http")
                         .setHostname("www.example.com")
                         .setPort("80")
                         .setPathParam("/path")
                         .setQueryParam("query")
                         .build();

    std::cout << "Protocol: " << url.protocol << "\n";
    std::cout << "Hostname: " << url.hostname << "\n";
    std::cout << "Port: " << url.port << "\n";
    std::cout << "Path: " << url.pathParam << "\n";
    std::cout << "Query: " << url.queryParam << "\n";

    return 0;
}