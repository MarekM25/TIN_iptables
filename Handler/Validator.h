//
// Created by mmmoraczynski on 20.12.15.
//

#ifndef TIN_IPTABLES_VALIDATOR_H
#define TIN_IPTABLES_VALIDATOR_H

#include <arpa/inet.h>
#include <json/json.h>
#include <limits>
#include "handler.h"

class Validator
{
    bool validateIp(Json::Value ip);
    bool isUIntShort(Json::Value);
    bool validateMac(Json::Value);
    bool validateChainType(Json::Value chainType);

public:
    bool validate(Json::Value jsonRequest);
};

#endif //TIN_IPTABLES_VALIDATOR_H
