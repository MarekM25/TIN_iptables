//
// Created by anowikowski on 10.12.15.
//

#ifndef TIN_IPTABLES_HTTPREQUEST_H
#define TIN_IPTABLES_HTTPREQUEST_H
#include <map>
#include <string>
#include <vector>
#include "HttpHeader.h"
#include "HttpHeaderCollection.h"

enum HttpRequestMethod
{
    GET,
    POST,
    OPTIONS,
    HEAD,
    PUT,
    DELETE,
    TRACE,
    CONNECT,
    LINK,
    UNLINK,
};


class HttpRequest
{
public:
    unsigned int GetContentLength();
    void SetRawHttpRequest(std::string httpRequestString);
    void SetData(std::string data);
    HttpRequestMethod GetMethod();
    HttpHeader GetHeader(std::string sHeaderName);
    void AddHeader(HttpHeader httpHeader);
    bool IsHeaderPresent(const std::string &sHeaderName);
    void Clear();
    std::string GetData();
    void AddData(const std::string &sData);
    std::string GetPath();
private:
    static const std::string m_sNewLineString;
    static const std::string m_sContentLengthHeaderName;
    static const std::string m_sHostHeaderName;
    std::string m_sData;
    std::string m_sHttpVersion;
    HttpRequestMethod m_method;
    std::string m_sPath;
    HttpHeaderCollection m_httpHeaderCollection;
    void SetHttpMethod(HttpRequestMethod method);
    void SetHttpMethod(const std::string &method);
    HttpRequestMethod MapHttpRequestMethod(const std::string &method);
};


#endif //TIN_IPTABLES_HTTPREQUEST_H
