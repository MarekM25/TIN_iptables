//
// Created by anowikowski on 17.12.15.
//

#ifndef TIN_IPTABLES_COMMANDLINEARGUMENTCOLLECTION_H
#define TIN_IPTABLES_COMMANDLINEARGUMENTCOLLECTION_H

#include <string>
#include <vector>
#include "CommandLineArgument.h"


class CommandLineArgumentCollection
{
public:
    CommandLineArgumentCollection();
    void Parse(int argc, char *argv[]);
    void Clear();
    bool IsArgumentSet(std::string sArgumentName);
    std::string GetArgumentValue(std::string sArgumentName);
private:
    std::vector<CommandLineArgument> m_arguments;
    static const std::string m_sArgumentNameValueSeparator;
    static const std::string m_sArgumentPrefix;
};


#endif //TIN_IPTABLES_COMMANDLINEARGUMENTCOLLECTION_H
