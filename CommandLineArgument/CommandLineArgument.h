//
// Created by anowikowski on 17.12.15.
//

#ifndef TIN_IPTABLES_COMMANDLINEARGUMENT_H
#define TIN_IPTABLES_COMMANDLINEARGUMENT_H

#include <string>

class CommandLineArgument
{
public:
    CommandLineArgument(std::string sArgumentName, std::string sArgumentValue);
    std::string GetArgumentName();
    void SetArgumentName(const std::string &sArgumentName);
    std::string GetArgumentValue();
    void SetArgumentValue(const std::string &sArgumentValue);
private:
    std::string m_sArgumentName;
    std::string m_sArgumentValue;
};


#endif //TIN_IPTABLES_COMMANDLINEARGUMENT_H
