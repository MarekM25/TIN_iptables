//
// Created by anowikowski on 16.12.15.
//

#ifndef TIN_IPTABLES_HTTPHEADER_H
#define TIN_IPTABLES_HTTPHEADER_H

#include <string>

class HttpHeader
{
public:
    HttpHeader(std::string sHeaderName, std::string sHeaderValue);
    HttpHeader(std::string sRawHeader);
    std::string GetHeaderName();
    void SetHeaderName(const std::string &sHeaderName);
    std::string GetHeaderValue();
    void SetHeaderValue(const std::string &sHeaderValue);
private:
    static const std::string m_sHttpHeaderNameValueDelimiter;
    std::string m_sHeaderName;
    std::string m_sHeaderValue;
};


#endif //TIN_IPTABLES_HTTPHEADER_H
