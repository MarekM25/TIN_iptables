//
// Created by anowikowski on 17.12.15.
//

#ifndef TIN_IPTABLES_COMMANDLINEEXCEPTION_H
#define TIN_IPTABLES_COMMANDLINEEXCEPTION_H

#include <exception>

namespace exception
{
    namespace command_line_argument
    {
        class command_line_argument_exception : public std::exception { };
        class argument_not_found : public command_line_argument_exception { };
        class invalid_argument_format : public command_line_argument_exception { };
        class argument_already_present : public command_line_argument_exception { };
    }
}

#endif //TIN_IPTABLES_COMMANDLINEEXCEPTION_H
