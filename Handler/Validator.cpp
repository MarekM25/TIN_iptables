//
// Created by mmoraczynski on 20.12.15.
//

#include "Validator.h"
#include "../Logger/Logger.h"

bool Validator::validateIp( Json::Value ip )
{
    if ( !ip.isString() )
    {
        LOG_ERR("Json Validator: IP incorrect value");
        return false;
    }

    sockaddr_in sa;

    return inet_pton( AF_INET, ip.asString().c_str(), &( sa.sin_addr ) ) != 0;
}

bool Validator::validateChainType( Json::Value chainType )
{
    if ( !chainType.isUInt() )
    {
        LOG_ERR("Json Validator: ChainType incorrect value");
        return false;
    }
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
    {
        LOG_ERR("Json Validator: MAC address incorrect value");
        return false;
    }

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
    {
        LOG_ERR("Json Validator: Unexpected Null value");
        return false;
    }

    if (!jsonRequest["command"].isInt() ||
                !(jsonRequest["command"].asInt()>=0 &&
            jsonRequest["command"].asInt()<= commandType::RAW ))
    {
        LOG_ERR("Json Validator: Unknown command");
        return false;
    }

    int command = jsonRequest["command"].asInt();
    switch (command)
    {
        case LOGIN_INIT:
            if (jsonRequest["params"]["username"].isNull())
            {
                LOG_ERR("Json Validator: LOGIN_INIT incorrect params value");
                return false;
            }
            break;
        case DELETE_RULE:
            if (jsonRequest["params"]["chainType"].isNull() ||
                    jsonRequest["params"]["line"].isNull())
            {
                LOG_ERR("Json Validator: DELETE_RULE incorrect params value");
                return false;
            }
            if (!validateChainType(jsonRequest["params"]["chainType"]) ||
                    !isUIntShort(jsonRequest["params"]["line"]))
            {
                LOG_ERR("Json Validator: DELETE_RULE incorrect params value");
                return false;
            }
            break;
        case BLOCK_IP:
            if (jsonRequest["params"]["chainType"].isNull() ||
                jsonRequest["params"]["ip"].isNull())
            {
                LOG_ERR("Json Validator: BLOCK_IP incorrect params value");
                return false;
            }
            if (!validateChainType(jsonRequest["params"]["chainType"]) ||
                !validateIp(jsonRequest["params"]["ip"]))
            {
                LOG_ERR("Json Validator: BLOCK_IP incorrect params value");
                return false;
            }
            break;
        case BLOCK_TCP_PORT:
            if (jsonRequest["params"]["chainType"].isNull() ||
                jsonRequest["params"]["tcpPort"].isNull())
            {
                LOG_ERR("Json Validator: BLOCK_TCP_PORT incorrect params value");
                return false;
            }
            if (!validateChainType(jsonRequest["params"]["chainType"]) ||
                !isUIntShort(jsonRequest["params"]["tcpPort"]))
            {
                LOG_ERR("Json Validator: BLOCK_TCP_PORT incorrect params value");
                return false;
            }
            break;
        case BLOCK_UDP_PORT:
            if (jsonRequest["params"]["chainType"].isNull() ||
                jsonRequest["params"]["udpPort"].isNull())
            {
                LOG_ERR("Json Validator: BLOCK_UDP_PORT incorrect params value");
                return false;
            }
            if (!validateChainType(jsonRequest["params"]["chainType"]) ||
                !isUIntShort(jsonRequest["params"]["udpPort"]))
            {
                LOG_ERR("Json Validator: BLOCK_UDP_PORT incorrect params value");
                return false;
            }
            break;
        case BLOCK_INCOMING_MAC:
            if (jsonRequest["params"]["mac"].isNull())
            {
                LOG_ERR("Json Validator: BLOCK_INCOMING_MAC incorrect params value");
                return false;
            }
            if (!validateMac(jsonRequest["params"]["mac"]))
            {
                LOG_ERR("Json Validator: BLOCK_INCOMING_MAC incorrect params value");
                return false;
            }
            break;
        case RAW:
            if (jsonRequest["params"]["mac"].isNull())
            {
                LOG_ERR("Json Validator: RAW incorrect params value");
                return false;
            }
            if (!validateMac(jsonRequest["params"]["mac"]))
            {
                LOG_ERR("Json Validator: RAW incorrect params value");
                return false;
            }
            break;

    }
    return true;
}
