//
// Created by anowikowski on 14.12.15.
//

#ifndef TIN_IPTABLES_HTTPRESPONSE_H
#define TIN_IPTABLES_HTTPRESPONSE_H

#include <string>
#include <map>

enum HttpResponseStatus
{
    OK_200,
    BAD_REQUEST_400,
    NOT_FOUND_404,
    METHOD_NOT_ALLOWED_405,
    INTERNAL_SERVER_ERROR_500,
};

class HttpResponse
{
public:
    HttpResponse();
    ~HttpResponse();
    std::string GetData();
    void SetData(const std::string &data);
    void SetStatus(HttpResponseStatus status);
    void SetHttpVersion(std::string httpVersion);
    std::string GetHttpVersion();
    HttpResponseStatus GetStatus();
    static int MapHttpResponseStatusToInt(HttpResponseStatus status);
    static std::string MapHttpResponseStatusToString(HttpResponseStatus status);
    void SetHeader(const std::string &sHeaderName, const std::string &sHeaderValue);
    void SetDefaultHttpVersion();
    std::map<std::string, std::string>& GetHeaders();
private:
    std::string m_sData;
    std::string m_sHttpVersion;
    std::map<std::string, std::string> m_headers;
    static const std::string m_sDefaultHttpVersion;
    HttpResponseStatus m_status;
};


#endif //TIN_IPTABLES_HTTPRESPONSE_H
