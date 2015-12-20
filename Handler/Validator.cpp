//
// Created by mmoraczynski on 20.12.15.
//

#include "Validator.h"

bool Validator::validateIp( Json::Value ip )
{
    if ( !ip.isString() )
        return false;

    sockaddr_in sa;

    return inet_pton( AF_INET, ip.asString().c_str(), &( sa.sin_addr ) ) != 0;
}

bool Validator::validateChainType( Json::Value chainType )
{
    if ( !chainType.isUInt() )
        return false;

    unsigned int chain = chainType.asUInt();

    return chain == 0 || chain == 1;
}

bool Validator::isUIntShort( Json::Value value )
{
    if ( !value.isUInt() )
        return false;

    unsigned short x = ( unsigned short ) value.asUInt();

    return x == value.asUInt();
}

bool Validator::validateMac( Json::Value value )
{
    if ( !value.isString() || value.asString().size() != 17 )
        return false;

    std::string mac = value.asString();

    for( int i = 0; i < 17; i++ )
    {
        if( i % 3 != 2 && !isxdigit( mac[ i ] ) )
            return false;
        if( i % 3 == 2 && mac[ i ] != ':' )
            return false;
    }

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
                !(jsonRequest["command"].asInt()>=0 &&
            jsonRequest["command"].asInt()<= commandType::RAW ))
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
