//
// Created by mmoraczynski on 15.12.15.
//
#include "auth.h"
#include <functional>
#include <stdlib.h>
#include <algorithm>
#include "../Handler/Validator.h"
#include "../Logger/Logger.h"

const size_t Authorization::m_challengeLength = 24;
const int Authorization::m_cleanupAuthorizationMapEvery = 100;

char* Authorization::strToMd5(std::string toHash)
{
    unsigned char digest[16];
    char *out = (char*)malloc(33);
    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5, toHash.c_str(), toHash.length());
    MD5_Final(digest, &md5);

    for (int i = 0; i < 16; ++i)
    {
        snprintf(&(out[i*2]), 16*3, "%02x", (unsigned int)digest[i]);
    }

    return out;
}


std::string Authorization::generateChallenge()
{
    srand( time( NULL ) );
    auto randchar = []() -> char
    {
        const char charset[] =
                "0123456789"
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(this->m_challengeLength,0);
    std::generate_n( str.begin(), this->m_challengeLength , randchar );
    return str;
}


Json::Value Authorization::loginInit(std::string username)
{
    Json::Value response;
    Configuration &config = Configuration::getInstance();

    srand((unsigned int)time(NULL));

    int r = rand() % this->m_cleanupAuthorizationMapEvery;
    if (r == this->m_cleanupAuthorizationMapEvery - 1)
    {
        this->cleanupAuthorizationMap();
    }

    LOG_ACS(username, "Authorization request received");

    if (!config.getUserPassword(username).empty()) {
        std::string challenge = generateChallenge();
        response["error_code"] = responseCode::RESPONSE_CODE_OK;
        response["error_message"] = "OK";
        response["challenge"] = challenge;
    }
    else
    {
        response["error_code"] = responseCode::RESPONSE_CODE_INVALID_USERNAME;
        response["error_message"] = "Specified username not exist";
        response["challenge"] = "";
        LOG_ACS(username, "FAILED: user does not exist");
    }
    return response;
}


bool Authorization::authorize(std::string challenge, std::string hash)
{
    Configuration &config = Configuration::getInstance();

    if (!this->checkIfChallengeInMap(challenge))
    {
        return false;
    }

    std::pair<std::string, time_t> usernameSessionLastUpdateTimePair = this->m_usernameChallengeMap.find(challenge)->second;

    std::string password = config.getUserPassword(usernameSessionLastUpdateTimePair.first);
    std::string localHash = strToMd5(password + challenge);

    srand((unsigned int)time(NULL));

    int r = rand() % this->m_cleanupAuthorizationMapEvery;
    if (r == this->m_cleanupAuthorizationMapEvery - 1)
    {
        this->cleanupAuthorizationMap();
    }

    return localHash == hash && !this->isSessionTimedOut(usernameSessionLastUpdateTimePair.second);
}

void Authorization::updateMap(std::string oldchallenge, std::string newchallenge)
{
    std::map<std::string, std::pair<std::string, time_t>>::iterator it = m_usernameChallengeMap.find(oldchallenge);
    if (it != m_usernameChallengeMap.end())
    {
        std::string username = it->second.first;
        m_usernameChallengeMap.erase(it);
        this->insertToMap(newchallenge, username);
    }
}

bool Authorization::checkIfChallengeInMap(std::string challenge)
{
    std::map<std::string, std::pair<std::string, time_t>>::iterator it = m_usernameChallengeMap.find(challenge);
    return it != m_usernameChallengeMap.end();
}

void Authorization::insertToMap(std::string challenge, std::string username)
{
    m_usernameChallengeMap.insert(std::pair<std::string, std::pair<std::string, time_t>>(challenge, std::pair<std::string, time_t>(username, time(NULL))));
}

void Authorization::removeFromMap(std::string challenge)
{
    std::map<std::string, std::pair<std::string, time_t>>::iterator it = m_usernameChallengeMap.find(challenge);
    if (it != m_usernameChallengeMap.end())
        m_usernameChallengeMap.erase(it);
}

bool  Authorization::isSessionTimedOut(time_t sessionLastUpdateTime)
{
    Configuration &config = Configuration::getInstance();
    time_t now;
    time(&now);

    double seconds = difftime(now, sessionLastUpdateTime);

    return seconds > config.getSessionTimeout();
}

void Authorization::cleanupAuthorizationMap()
{
    int cleanedSessions = 0;
    std::map<std::string, std::pair<std::string, time_t>>::iterator it = this->m_usernameChallengeMap.begin();
    for ( ; it != this->m_usernameChallengeMap.end(); )
    {
        if (this->isSessionTimedOut(it->second.second))
        {
            this->m_usernameChallengeMap.erase(it++);
            cleanedSessions++;
        }
        else
        {
            ++it;
        }
    }

    LOG("Successfully cleaned up " + std::to_string(cleanedSessions) + " sessions.");
}
