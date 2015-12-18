//
// Created by Pawel Wiszenko on 18.12.2015.
//

#ifndef TIN_IPTABLES_IPTABLESEXCEPTION_H
#define TIN_IPTABLES_IPTABLESEXCEPTION_H

#include <exception>

namespace exception
{
    namespace iptables
    {
        class iptables_exception : public std::exception { };
        class invalid_command : public iptables_exception { };
        class exec_error : public iptables_exception { };
    }
}

#endif //TIN_IPTABLES_IPTABLESEXCEPTION_H
