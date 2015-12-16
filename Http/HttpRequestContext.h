//
// Created by anowikowski on 16.12.15.
//

#ifndef TIN_IPTABLES_HTTPREQUESTCONTEXT_H
#define TIN_IPTABLES_HTTPREQUESTCONTEXT_H

#include <string>
#include "HttpRequest.h"

class HttpRequestContext
{
public:
    HttpRequestContext();
    ~HttpRequestContext();
    void SetHttpRequest(HttpRequest httpRequest);
    HttpRequest& GetHttpRequest();
    void SetClientIpAddress(std::string sourceIpAddress);
    std::string GetClientIpAddress();
protected:
    std::string m_sClientIpAddress;
    HttpRequest m_httpRequest;
};


#endif //TIN_IPTABLES_HTTPREQUESTCONTEXT_H
