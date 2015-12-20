//
// Created by mmmoraczynski on 20.12.15.
//

#ifndef TIN_IPTABLES_VALIDATOR_H
#define TIN_IPTABLES_VALIDATOR_H

#include <json/json.h>

class Validator
{
    bool validateIp(Json::Value ip);
    bool validatePort(Json::Value port);
    bool validateChainType(Json::value chainType);
public:
    bool validate(Json::Value jsonRequest);
};

#endif //TIN_IPTABLES_VALIDATOR_H
