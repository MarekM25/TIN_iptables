#include <iostream>
#include <unistd.h>
#include <signal.h>

#include "json/json.h"

#include "Configuration/Configuration.h"
#include "Http/HttpServer.h"

#include "Logger/Logger.h"
#include "Handler/handler.h"

#include "Exception/CommandLineArgumentException.h"
#include "Exception/ConfigurationException.h"
#include "CommandLineArgument/CommandLineArguments.h"

static bool isStopRequested = false;
static std::string defaultConfigFilePath = "iptables.conf";

void displayUsage(std::string executableName)
{
    std::cout << "Usage: ./" << executableName << " -" << CommandLineArguments::ConfigFilePathArgumentName << "=[path_to_config_file] [args]" << std::endl;
    std::cout << "Available arguments:" << std::endl;
    std::cout << "\t-" << CommandLineArguments::LogLevelArgumentName << "=[e][i][a] (ex. -" << CommandLineArguments::LogLevelArgumentName << "=ei would log errors and info)" << std::endl;
}

void signalHandler(int signal)
{
    LOG("Received signal: " + std::to_string(signal));
    isStopRequested = true;
}

int main(int argc, char *argv[])
{
    signal(SIGINT, signalHandler);

    CommandLineArguments commandLineArguments;

    try
    {
        commandLineArguments.Parse(argc, argv);
    }
    catch (const exception::command_line_argument::command_line_argument_exception &e)
    {
        displayUsage(argv[0]);
        return 0;
    }
    
    Logger<FileLogPolicy> &loggerInstance = Logger<FileLogPolicy>::getInstance();

    if (commandLineArguments.IsAccessLoggingSet())
    {
        loggerInstance.enableAccessLogging();
    }

    if (commandLineArguments.IsErrorLoggingSet())
    {
        loggerInstance.enableErrorLogging();
    }

    if (commandLineArguments.IsInfoLoggingSet())
    {
        loggerInstance.enableInfoLogging();
    }

     // Example of use jsoncpp

    std::ifstream json_test;
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
    std::cout << value;

    Configuration &configurationInstance = Configuration::getInstance();
    try
    {
        if ( commandLineArguments.IsConfigFilePathSet() )
        {
            configurationInstance.initialize( commandLineArguments.GetConfigFilePath() );
        }
        else
        {
            configurationInstance.initialize( defaultConfigFilePath );
        }
    }
    catch ( const exception::configuration::invalid_config_path &e )
    {
        std::cout << "Error loading configuration file" << std::endl;
        return 0;
    }
    catch ( const exception::configuration::invalid_blacklist_path &e )
    {
        std::cout << "Error loading blacklist file" << std::endl;
        return 0;
    }
    catch ( const exception::configuration::invalid_users_path &e )
    {
        std::cout << "Error loading users file" << std::endl;
        return 0;
    }

    loggerInstance.initialize(configurationInstance.getLogPath());
    LOG("initialized");


    /*
     * example of ip address checking
     *
    cout << configurationInstance.isIPAddressBlocked( "192.168.1.101" ) << endl;
    cout << configurationInstance.isIPAddressBlocked( "190.168.1.101" ) << endl;
    cout << configurationInstance.isIPAddressBlocked( "88.23.1.115" ) << endl;
    */

    /*
     * example of iptables commands
     */
    IPTablesExecutor iptexec;
    try
    {
        //std::cout << iptexec.rawCommand( "iptables -L" ) << std::endl;
        std::cout << iptexec.rawCommand( "pwd -L" ) << std::endl;
        //std::cout << iptexec.rawCommand( "iptables -L | rm -rf *" ) << std::endl;
        //std::cout << iptexec.rawCommand( "iptables -D OUTPUT 3" ) << std::endl;
    }
    catch ( const exception::iptables::invalid_command &e )
    {
        std::cout << "Error: invalid command" << std::endl;
    }
    catch ( const exception::iptables::exec_error &e )
    {
        std::cout << "Error executing command" << std::endl;
    }


    HttpServer server;
    server.SetPort(configurationInstance.getServerPort());

    if (configurationInstance.isServerIpAddressSet())
    {
        server.SetListeningIpAddress(configurationInstance.getServerIpAddress());
        LOG("Server IP Address: ", configurationInstance.getServerIpAddress());
    }
    else
    {
        LOG("Server IP Address: *");
    }

    server.SetMaxConnectionQueueLength(8);
    server.SetSendTimeout(configurationInstance.getTransmissionTimeout());
    server.SetReceiveTimeout(configurationInstance.getTransmissionTimeout());

    Handler httpRequestHandler;


    server.SetHttpRequestHandlerContextObject(&httpRequestHandler);

    LOG("Server Port: ", configurationInstance.getServerPort());

    server.Start();

    LOG_ACS("Hello World request");

    while(!isStopRequested)
    {
        sleep(1);
    }

    LOG("Stopping server");

    server.Stop();

    LOG("Server stopped");

    return 0;
}