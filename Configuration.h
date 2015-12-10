//
// Created by anowikowski on 10.12.15.
//

#ifndef TIN_IPTABLES_CONFIGURATION_H
#define TIN_IPTABLES_CONFIGURATION_H

#include <string>

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
    Configuration(Configuration const&);
    string serverIpAddress;
    unsigned short serverPort;
};


#endif //TIN_IPTABLES_CONFIGURATION_H
