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
#include <time.h>

class Authorization
{
    const size_t m_challengeLength = 24;
    static const int m_cleanupAuthorizationMapEvery;
    std::map<std::string,std::pair<std::string, time_t>> m_usernameChallengeMap;
    char* strToMd5(std::string toHash);
    void cleanupAuthorizationMap();
    bool isSessionTimedOut(time_t sessionLastUpdateTime);
public:
    Json::Value loginInit(std::string username);
    std::string generateChallenge();
    bool authorize(std::string challenge, std::string hash);
    bool checkIfChallengeInMap(std::string challenge);
    void updateMap(std::string oldChallenge, std::string newChallenge);
    void insertToMap(std::string challenge, std::string username);
    void removeFromMap(std::string challenge);
};

#endif //TIN_IPTABLES_AUTH_H