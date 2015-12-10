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
    void Initialize(string configurationFilePath);
private:
    Configuration();
    Configuration(Configuration const&);
};


#endif //TIN_IPTABLES_CONFIGURATION_H
