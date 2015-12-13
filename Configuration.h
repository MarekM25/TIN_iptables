//
// Created by anowikowski on 10.12.15.
//

#ifndef TIN_IPTABLES_CONFIGURATION_H
#define TIN_IPTABLES_CONFIGURATION_H

#include <string>
#include <fstream>
#include <map>

using namespace std;

class Configuration
{
public:
    static Configuration& getInstance();
    ~Configuration();
    void initialize(string configurationFilePath);
    string getServerIpAddress();
    unsigned short getServerPort();

private:
    Configuration();
    //Configuration(Configuration const&);
    bool parseConfigFile(string& configFile);
    string trim( string source );

    map< string, string > params;

    string serverIpAddress;
    unsigned short serverPort;

    string listenIpAddress;
    unsigned short listenPort;
    unsigned short sessionTimeout;          // unsigned short -> max ~18h
    unsigned short transmissionTimeout;
    string logPath;
    string usersFilePath;
};


#endif //TIN_IPTABLES_CONFIGURATION_H
