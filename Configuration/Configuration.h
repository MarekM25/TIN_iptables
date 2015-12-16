//
// Created by anowikowski on 10.12.15.
//

#ifndef TIN_IPTABLES_CONFIGURATION_H
#define TIN_IPTABLES_CONFIGURATION_H

#include <arpa/inet.h>
#include <string>
#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>

using namespace std;

class Configuration
{
public:
    static Configuration& getInstance();
    ~Configuration();
    void initialize( string configurationFilePath );
    bool isIPAddressBlocked( const string& ipAddress );
    bool isServerIpAddressSet();

    string getHostName();
    string getServerIpAddress();
    unsigned short getServerPort();
    unsigned short getSessionTimeout();
    unsigned short getTransmissionTimeout();
    string getLogPath();
    string getUsersFilePath();
    string getBlacklistFilePath();
    string getUserPassword(string username);

private:
    Configuration();
    //Configuration( Configuration const& );
    bool parseConfigFile( string& configurationFilePath );
    bool parseBlacklistFile();
    bool parseUsersFile();
    uint32_t IPToUInt( const string& ipAddress );

    vector< pair< string, string > > mBlacklist;

    unordered_map<string, string> mUsers;

    string mHostName;
    string mServerIpAddress;
    unsigned short mServerPort;
    unsigned short mSessionTimeout;          // unsigned short -> max ~18h
    unsigned short mTransmissionTimeout;
    string mLogPath;
    string mUsersFilePath;
    string mBlacklistFilePath;
    bool mIsServerIpAddressSet;
};


#endif //TIN_IPTABLES_CONFIGURATION_H
