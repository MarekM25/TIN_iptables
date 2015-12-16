#include <iostream>
#include <unistd.h>

#include "json/json.h"

#include "Configuration/Configuration.h"
#include "Http/HttpServer.h"
#include "IPTables/IPTablesExecutor.h"

#include "Logger/Logger.h"
#include "Authorization/auth.h"


using namespace std;

HttpResponse httpRequestHandler(HttpRequest httpRequest)
{
    HttpResponse httpResponse;
    Json::Reader reader;
    Json::FastWriter writer;
    Json::Value jsonRequest, jsonResponse;
    if (reader.parse(httpRequest.GetData(),jsonRequest))
    {
        Configuration &config= Configuration::getInstance();
        config.initialize("iptables.conf");
        if (config.isIPAddressBlocked("192.0.0.1"))
        {
            jsonResponse["error_code"] = 20;
            jsonResponse["error_message"] = "You are not authorized to use this server.";
            jsonResponse["challange"] = "";
        }
        else
        {
            IPTablesExecutor iptexec;
            switch (jsonRequest["command"].asInt())
            {
                case LOGIN_INIT:
                    break;
                case LOGIN_REQUEST:
                    break;
                case LOGOUT:
                    break;
                case GET_ALL_RULES:
                    iptexec.executeCommand( GET_ALL_RULES );
                    break;
                case DELETE_RULE:

                    break;
                case BLOCK_IP:

                    break;
                case BLOCK_TCP_PORT:

                    break;
                case BLOCK_UDP_PORT:

                    break;
                case BLOCK_INCOMING_MAC:

                    break;
                case RAW:

                    break;

            }
        }
    }
    else
    {
        jsonResponse["error_code"] = 10;
        jsonResponse["error_message"] = "Request was not in JSON format";
    }
    writer.write(jsonResponse);
    httpResponse.SetData(jsonResponse.asString());
    return httpResponse;
}

int main()
{
    LOG("initialized");

     // Example of use jsoncpp

    ifstream json_test;
    json_test.open("test.json", std::ios::in | std::ios::out);

    Json::Value test_value;
    Json::Reader reader;
    if(reader.parse(json_test, test_value))

    std::cout<<test_value["widget"]["window"]["title"].asString();
    Json::StyledStreamWriter  writer;
    Json::FastWriter fastWriter;

    Json::Value value;
    int b[2];
    Json::Value arrayTest(Json::arrayValue);
    b[0]= 3;
    b[1] = 34;
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
    server.SetListeningIpAddress(configurationInstance.getServerIpAddress());
    server.SetHttpRequestHandler(httpRequestHandler);

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