//
// Created by anowikowski on 10.12.15.
//

#ifndef TIN_IPTABLES_CONFIGURATION_H
#define TIN_IPTABLES_CONFIGURATION_H

#include <string>
#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>

class Configuration
{
public:
    static Configuration& getInstance();
    Configuration(Configuration const&) = delete;
    void operator=(Configuration const&) = delete;

    void initialize( std::string configurationFilePath );
    bool isIPAddressBlocked( const std::string& ipAddress );
    bool isServerIpAddressSet();

    std::string getHostName();
    std::string getServerIpAddress();
    unsigned short getServerPort();
    unsigned short getSessionTimeout();
    unsigned short getTransmissionTimeout();
    std::string getLogPath();
    std::string getUsersFilePath();
    std::string getBlacklistFilePath();
    std::string getUserPassword( std::string username );

private:
    Configuration();
    //Configuration( Configuration const& );
    bool parseConfigFile( std::string& configurationFilePath );
    bool parseBlacklistFile();
    bool parseUsersFile();
    uint32_t IPToUInt( const std::string& ipAddress );

    std::vector< std::pair< std::string, std::string > > mBlacklist;

    std::unordered_map< std::string, std::string > mUsers;

    std::string mHostName;
    std::string mServerIpAddress;
    unsigned short mServerPort;
    unsigned short mSessionTimeout;          // unsigned short -> max ~18h
    unsigned short mTransmissionTimeout;
    std::string mLogPath;
    std::string mUsersFilePath;
    std::string mBlacklistFilePath;
    bool mIsServerIpAddressSet;
};


#endif //TIN_IPTABLES_CONFIGURATION_H
