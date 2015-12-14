//
// Created by anowikowski on 13.12.15.
//

#ifndef TIN_IPTABLES_STRINGEXTENSIONS_H
#define TIN_IPTABLES_STRINGEXTENSIONS_H

#include <string>

namespace string_extensions
{
    bool ends_with(const std::string &str, const std::string &ending);
    unsigned int stoui(const std::string &s);
    std::string tolower(const std::string &str);
}

#endif //TIN_IPTABLES_STRINGEXTENSIONS_H
