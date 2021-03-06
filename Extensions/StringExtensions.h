//
// Created by anowikowski on 13.12.15.
//

#ifndef TIN_IPTABLES_STRINGEXTENSIONS_H
#define TIN_IPTABLES_STRINGEXTENSIONS_H

#include <string>
#include <vector>

namespace string_extensions
{
    bool ends_with(const std::string &str, const std::string &ending);
    unsigned int stoui(const std::string &s);
    unsigned short stous(const std::string &s);
    std::string trim( const std::string& sourceString );
    std::string tolower(const std::string &str);
    std::vector<std::string> split(const std::string &s);
}

#endif //TIN_IPTABLES_STRINGEXTENSIONS_H
