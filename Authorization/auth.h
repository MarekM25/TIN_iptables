//
// Created by mmoraczynski on 15.12.15.
//

#ifndef TIN_IPTABLES_AUTH_H
#define TIN_IPTABLES_AUTH_H
#include <string>
#include "md5.h"
class Authorization
{
    char* strToMd5(std::string toHash);
    std::string generateChallenge();

public:

    std::string loginInit(std::string ipAddress); //JsonResponse będzie zwracany
    std::string loginRequest(std::string challange);

    void logout(std::string challange);

};

#endif //TIN_IPTABLES_AUTH_H