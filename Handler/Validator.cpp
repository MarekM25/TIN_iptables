//
// Created by mmoraczynski on 20.12.15.
//

#include "Validator.h"

bool Validator::validateIp(Json::Value ip)
{
    //to do
    return true;
}

bool Validator::validateChainType(Json::Value chainType)
{
    //to do
    return true;
}

bool Validator::isUIntShort(Json::Value value)
{
    //to do
    return true;
}

bool Validator::validateMac(Json::Value)
{
    //to do
    return true;
}


bool Validator::validate(Json::Value jsonRequest)
{
    if (jsonRequest["command"].isNull() ||
            jsonRequest["hash"].isNull() ||
            jsonRequest["challenge"].isNull() ||
            jsonRequest["params"].isNull())
        return false;
    if (!jsonRequest["command"].isInt() ||
            jsonRequest["command"].asInt()<0 ||
            jsonRequest["command"].asInt()> std::numeric_limits<enum commandType>::max())
        return false;
    int command = jsonRequest["command"].isInt();
    switch (command)
    {
        case LOGIN_INIT:
            if (jsonRequest["params"]["username"].isNull())
                return false;
            break;
        case DELETE_RULE:
            if (jsonRequest["params"]["chainType"].isNull() ||
                    jsonRequest["params"]["line"].isNull())
                return false;
            if (!validateChainType(jsonRequest["params"]["chainType"]) ||
                    !isUIntShort(jsonRequest["params"]["line"]))
                return false;
            break;
        case BLOCK_IP:
            if (jsonRequest["params"]["chainType"].isNull() ||
                jsonRequest["params"]["ip"].isNull())
                return false;
            if (!validateChainType(jsonRequest["params"]["chainType"]) ||
                !validateIp(jsonRequest["params"]["ip"]))
                return false;
            break;
        case BLOCK_TCP_PORT:
            if (jsonRequest["params"]["chainType"].isNull() ||
                jsonRequest["params"]["tcpPort"].isNull())
                return false;
            if (!validateChainType(jsonRequest["params"]["chainType"]) ||
                !isUIntShort(jsonRequest["params"]["tcpPort"]))
                return false;
            break;
        case BLOCK_UDP_PORT:
            if (jsonRequest["params"]["chainType"].isNull() ||
                jsonRequest["params"]["udpPort"].isNull())
                return false;
            if (!validateChainType(jsonRequest["params"]["chainType"]) ||
                !isUIntShort(jsonRequest["params"]["udpPort"]))
                return false;
            break;
        case BLOCK_INCOMING_MAC:
            if (jsonRequest["params"]["mac"].isNull())
                return false;
            if (!validateMac(jsonRequest["params"]["mac"]))
                return false;
            break;
        case RAW:
            if (jsonRequest["params"]["mac"].isNull())
                return false;
            if (!validateMac(jsonRequest["params"]["mac"]))
                return false;
            break;

    }
    return true;
}
