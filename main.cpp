#include <iostream>
#include <unistd.h>

#include "Configuration.h"
#include "HttpServer.h"

using namespace std;

int main()
{
    Configuration &configurationInstance = Configuration::getInstance();
    configurationInstance.initialize("iptables.conf");

    HttpServer server(configurationInstance.getServerIpAddress(), configurationInstance.getServerPort());

    server.start();

    cout << "Hello, World!" << endl;

    while(true) //NOT RECEIVED EXIT SIGNAL OR COMMAND
    {
        sleep(1);
    }

    server.stop();

    return 0;
}