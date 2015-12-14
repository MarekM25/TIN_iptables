#include <iostream>
#include <unistd.h>

#include "Configuration.h"
#include "Http/HttpServer.h"

using namespace std;

HttpResponse httpRequestHandler(HttpRequest httpRequest)
{
    HttpResponse httpResponse;
    return httpResponse;
}

int main()
{
    Configuration &configurationInstance = Configuration::getInstance();
    configurationInstance.initialize("iptables.conf");

    HttpServer server;
    server.setPort(configurationInstance.getServerPort());
    server.setListeningIpAddress(configurationInstance.getServerIpAddress());
    server.setHttpRequestHandler(httpRequestHandler);

    server.start();

    while(true) //NOT RECEIVED EXIT SIGNAL OR COMMAND
    {
        sleep(1);
    }

    server.stop();

    return 0;
}