//
// Created by anowikowski on 10.12.15.
//

#ifndef TIN_IPTABLES_HTTPREQUEST_H
#define TIN_IPTABLES_HTTPREQUEST_H
#include <map>
#include <string>

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
    HttpRequest();
    ~HttpRequest();
    unsigned int GetContentLength();
    std::string GetHeaderValue(std::string header);
    void SetRawHttpRequest(std::string httpRequestString);
    bool IsHeaderPresent(const std::string &headerName);
    void SetData(std::string data);
    HttpRequestMethod GetMethod();
    void Clear();
    std::string GetData();
private:
    static const std::string m_sNewLineString;
    static const std::string m_sHttpHeaderNameValueDelimiter;
    static const std::string m_sContentLengthHeaderName;
    static const std::string m_sHostHeaderName;
    std::map<std::string, std::string> m_headers;
    std::string m_sData;
    std::string m_sHttpVersion;
    HttpRequestMethod m_method;
    std::string m_sPath;
    std::pair<std::string, std::string> SplitHttpHeaderNameValue(const std::string &httpHeader);
    void SetHttpMethod(HttpRequestMethod method);
    void SetHttpMethod(const std::string &method);
    HttpRequestMethod MapHttpRequestMethod(const std::string &method);
};


#endif //TIN_IPTABLES_HTTPREQUEST_H
