//
// Created by anowikowski on 17.12.15.
//

#ifndef TIN_IPTABLES_COMMANDLINEARGUMENTS_H
#define TIN_IPTABLES_COMMANDLINEARGUMENTS_H


#include "CommandLineArgumentCollection.h"

#include <string>

class CommandLineArguments : public CommandLineArgumentCollection
{
public:
    void Parse(int argc, char *argv[]);
    bool IsAccessLoggingSet();
    bool IsErrorLoggingSet();
    bool IsInfoLoggingSet();
    bool IsConfigFilePathSet();
    std::string GetConfigFilePath();
    static const std::string LogLevelArgumentName;
    static const std::string AccessLoggingFlag;
    static const std::string ErrorLoggingFlag;
    static const std::string InfoLoggingFlag;
    static const std::string ConfigFilePathArgumentName;
};


#endif //TIN_IPTABLES_COMMANDLINEARGUMENTS_H
