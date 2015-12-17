//
// Created by mmoraczynski on 15.12.15.
//

#ifndef TIN_IPTABLES_AUTH_H
#define TIN_IPTABLES_AUTH_H
#include <string>
#include <json/json.h>
#include <fstream>
#include "md5.h"
#include "../Configuration/Configuration.h"
#include <iostream>
class Authorization
{
    const size_t m_challangeLength = 24;
    std::string m_userPath;
    char* strToMd5(std::string toHash);
public:
    Json::Value loginInit(std::string username);
    std::string generateChallenge();
    Json::Value loginRequest(std::string username,std::string hash, std::string challange);
    void logout(std::string challange);

};

#endif //TIN_IPTABLES_AUTH_H