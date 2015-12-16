//
// Created by mmoraczynski on 15.12.15.
//
#include "auth.h"
#include <functional>
#include <stdlib.h>
#include <algorithm>

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
    auto randchar = []() -> char
    {
        const char charset[] =
                "0123456789"
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(this->m_challangeLength,0);
    std::generate_n( str.begin(), this->m_challangeLength , randchar );
    return str;
}


Json::Value Authorization::loginInit(std::string username)
{
    Json::Value response;
    //check if exist
        std::string challange = generateChallenge();
        response["error_code"] = 0;
        response["error_message"] = "OK";
        response["challange"] = challange;
    //if not exist
        response["error_code"] = 21;
        response["error_message"] = "Specified username not exist";
        response["challange"] = "";
    return response;
}


void Authorization::saveToFile(std::string username, std::string challange)
{
    
}

void Authorization::updateInFile(std::string oldChallange, std::string newChallange)
{

}

void Authorization::removeInFile(std::string challange)
{

}

Json::Value Authorization::nextChallange(std::string challange)
{

}

void Authorization::logout(std::string challange)
{
    
}

Authorization::Authorization() 
{
    Configuration &config= Configuration::getInstance();
    m_userPath = config.getUsersFilePath();
}