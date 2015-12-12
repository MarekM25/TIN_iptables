#define LOGGING_LEVEL_1

#include <iostream>
#include <unistd.h>

#include "Configuration.h"
#include "HttpServer.h"

#include "Logger/Logger.h"


using namespace std;

int main()
{
    LOG("initialized");

    Configuration &configurationInstance = Configuration::getInstance();
    configurationInstance.initialize("iptables.conf");

    HttpServer server(configurationInstance.getServerIpAddress(), configurationInstance.getServerPort());

    LOG("Server IP Address: ", configurationInstance.getServerIpAddress(), " Server Port: ", configurationInstance.getServerPort());
    server.start();

    cout << "Hello, World!" << endl;
    LOG_ACS("Hello World request");

    while(true) //NOT RECEIVED EXIT SIGNAL OR COMMAND
    {
        LOG_ERR("FUCK, NOTHING HAPPENS :(");
        sleep(1);
    }

    server.stop();

    return 0;
}