#include <iostream>
#include <unistd.h>

#include "json/json.h"

#include "Configuration/Configuration.h"
#include "Http/HttpServer.h"
#include "IPTables/IPTablesExecutor.h"

#include "Logger/Logger.h"
#include "Authorization/auth.h"
#include "Handler/handler.h"

#include "Http/HttpRequestContext.h"


using namespace std;


int main()
{

    LOG("initialized");
     // Example of use jsoncpp

    ifstream json_test;
    json_test.open("test.json", std::ios::in | std::ios::out);
     Json::Value test_value;
    Json::Reader reader;
     if(reader.parse(json_test, test_value))

    std::cout<<test_value["hash"].asString();

    Json::Value value;
    Json::Value arrayTest(Json::arrayValue);
    Json::FastWriter fastWriter;
    value["test"] = 5;
    arrayTest.append(4);
    arrayTest.append(5);
    value["x"] =arrayTest;

    fastWriter.write(value);
    cout << value;


    Configuration &configurationInstance = Configuration::getInstance();
    configurationInstance.initialize("../iptables.conf");

    /*
     * example of ip address checking
     *
    cout << configurationInstance.isIPAddressBlocked( "192.168.1.101" ) << endl;
    cout << configurationInstance.isIPAddressBlocked( "190.168.1.101" ) << endl;
    cout << configurationInstance.isIPAddressBlocked( "88.23.1.115" ) << endl;
    */

    /*
     * example of iptables commands
     *
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
    */
    HttpServer server;
    server.SetPort(configurationInstance.getServerPort());

    if (configurationInstance.isServerIpAddressSet())
    {
        server.SetListeningIpAddress(configurationInstance.getServerIpAddress());
    }

    server.SetMaxConnectionQueueLength(8);
    server.SetSendTimeout(configurationInstance.getTransmissionTimeout());
    server.SetReceiveTimeout(configurationInstance.getTransmissionTimeout());

    Handler httpRequestHandler;


    server.SetHttpRequestHandlerContextObject(&httpRequestHandler);

    LOG("Server IP Address: ", configurationInstance.getServerIpAddress(), " Server Port: ", configurationInstance.getServerPort());
    server.Start();

    cout << "Hello, World!" << endl;


    LOG_ACS("Hello World request");

    while(true) //NOT RECEIVED EXIT SIGNAL OR COMMAND
    {
        //LOG_ERR("Some error occurred");
        sleep(1);
    }

    server.Stop();

    return 0;
}