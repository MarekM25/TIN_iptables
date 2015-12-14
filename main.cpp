#include <iostream>
#include <unistd.h>

#include "Configuration.h"
#include "Http/HttpServer.h"
#include "IPTablesExecutor.h"

#include "Logger/Logger.h"

using namespace std;

HttpResponse httpRequestHandler(HttpRequest httpRequest)
{
    HttpResponse httpResponse;
    return httpResponse;
}

int main()
{
    LOG("initialized");

    Configuration &configurationInstance = Configuration::getInstance();
    configurationInstance.initialize("iptables.conf");

    IPTablesExecutor iptexec;
    iptexec.executeCommand( GET_ALL_RULES );
    iptexec.executeCommand( DELETE_RULE, 1, INPUT );
    iptexec.executeCommand( DELETE_RULE, 4, OUTPUT );
    iptexec.executeCommand( BLOCK_IP, "192.168.1.101", INPUT );
    iptexec.executeCommand( BLOCK_IP, "192.168.1.104", OUTPUT );
    iptexec.executeCommand( BLOCK_TCP_PORT, 9999, OUTPUT );
    iptexec.executeCommand( BLOCK_UDP_PORT, 1234, INPUT );
    iptexec.executeCommand( BLOCK_INCOMING_MAC, "00:0F:EA:91:04:08" );
    iptexec.executeCommand( RAW, "pwd" );

    HttpServer server;
    server.setPort(configurationInstance.getServerPort());
    server.setListeningIpAddress(configurationInstance.getServerIpAddress());
    server.setHttpRequestHandler(httpRequestHandler);

    LOG("Server IP Address: ", configurationInstance.getServerIpAddress(), " Server Port: ", configurationInstance.getServerPort());
    server.start();

    cout << "Hello, World!" << endl;
    LOG_ACS("Hello World request");

    while(true) //NOT RECEIVED EXIT SIGNAL OR COMMAND
    {
        //LOG_ERR("Some error occurred");
        sleep(1);
    }

    server.stop();

    return 0;
}