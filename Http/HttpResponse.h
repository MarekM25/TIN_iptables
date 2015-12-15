//
// Created by anowikowski on 14.12.15.
//

#ifndef TIN_IPTABLES_HTTPRESPONSE_H
#define TIN_IPTABLES_HTTPRESPONSE_H

#include <string>

class HttpResponse
{
public:
    HttpResponse();
    ~HttpResponse();
    std::string GetData();
    std::string SetData(const std::string &data);
private:
    std::string m_sData;
};


#endif //TIN_IPTABLES_HTTPRESPONSE_H
