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
    const size_t m_challengeLength = 24;
    char* strToMd5(std::string toHash);
public:
    Json::Value loginInit(std::string username);
    std::string generateChallenge();
    bool authorize(std::string username,std::string hash, std::string challenge);
};

#endif //TIN_IPTABLES_AUTH_H