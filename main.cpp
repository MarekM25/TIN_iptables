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
#include "Exception/HttpException.h"

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
        return 1;
    }
    catch ( const exception::configuration::invalid_blacklist_path &e )
    {
        std::cout << "Error loading blacklist file" << std::endl;
        return 1;
    }
    catch ( const exception::configuration::invalid_users_path &e )
    {
        std::cout << "Error loading users file" << std::endl;
        return 1;
    }

    loggerInstance.initialize(configurationInstance.getLogPath());
    LOG("Initialized");

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

    try
    {
        server.Start();
    }
    catch (const exception::http::http_server_exception &e)
    {
        LOG_ERR("Error starting http server. Maybe another instance is already running.");
        return 1;
    }


    while(!isStopRequested && server.IsRunning())
    {
        sleep(1);
    }

    LOG("Stopping server");

    server.Stop();

    LOG("Server stopped");

    return 0;
}