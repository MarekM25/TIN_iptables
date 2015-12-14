//
// Created by anowikowski on 10.12.15.
//

#ifndef TIN_IPTABLES_HTTPREQUEST_H
#define TIN_IPTABLES_HTTPREQUEST_H

#include <map>

enum HttpRequestMethod
{
    GET,
    POST,
    UNKNOWN
};


class HttpRequest
{
public:
    HttpRequest(std::string httpRequestString);
    ~HttpRequest();
    unsigned int getContentLength();
    std::string getHeaderValue(std::string header);
    bool isHeaderPresent(const std::string &headerName);
    void setHttpRequestData(std::string httpRequestDataString);
private:
    const std::string newLineString = "\r\n";
    const std::string httpHeaderNameValueDelimiter = ": ";
    std::map<std::string, std::string> headers;
    std::string data;
    HttpRequestMethod method;
    std::string path;
};


#endif //TIN_IPTABLES_HTTPREQUEST_H
