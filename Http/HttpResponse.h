//
// Created by anowikowski on 14.12.15.
//

#ifndef TIN_IPTABLES_HTTPRESPONSE_H
#define TIN_IPTABLES_HTTPRESPONSE_H

#include <string>
#include <map>
#include "HttpHeaderCollection.h"

enum HttpResponseStatus
{
    OK = 200,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    INTERNAL_SERVER_ERROR = 500,
};

class HttpResponse
{
public:
    HttpResponse();
    std::string GetData();
    void SetData(const std::string &data);
    void SetStatus(HttpResponseStatus status);
    void SetHttpVersion(std::string httpVersion);
    std::string GetHttpVersion();
    HttpResponseStatus GetStatus();
    static std::string MapHttpResponseStatusToString(HttpResponseStatus status);
    void SetDefaultHttpVersion();
    HttpHeaderCollection& GetHttpHeaderCollection();
    void AddHeader(HttpHeader httpHeader);
private:
    std::string m_sData;
    std::string m_sHttpVersion;
    HttpHeaderCollection m_httpHeaderCollection;
    static const std::string m_sDefaultHttpVersion;
    HttpResponseStatus m_status;
};


#endif //TIN_IPTABLES_HTTPRESPONSE_H
