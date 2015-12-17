//
// Created by anowikowski on 17.12.15.
//

#include "CommandLineArguments.h"
#include "../Exception/CommandLineArgumentException.h"

const std::string CommandLineArguments::LogLevelArgumentName = "log";
const std::string CommandLineArguments::AccessLoggingFlag = "a";
const std::string CommandLineArguments::ErrorLoggingFlag = "e";
const std::string CommandLineArguments::InfoLoggingFlag = "i";
const std::string CommandLineArguments::ConfigFilePathArgumentName = "config";

bool CommandLineArguments::IsAccessLoggingSet()
{
    return this->IsArgumentSet(this->LogLevelArgumentName) && this->GetArgumentValue(this->LogLevelArgumentName).find(this->AccessLoggingFlag) != std::string::npos;
}

bool CommandLineArguments::IsErrorLoggingSet()
{
    return this->IsArgumentSet(this->LogLevelArgumentName) && this->GetArgumentValue(this->LogLevelArgumentName).find(this->ErrorLoggingFlag) != std::string::npos;
}

bool CommandLineArguments::IsInfoLoggingSet()
{
    return this->IsArgumentSet(this->LogLevelArgumentName) && this->GetArgumentValue(this->LogLevelArgumentName).find(this->InfoLoggingFlag) != std::string::npos;
}

bool CommandLineArguments::IsConfigFilePathSet()
{
    return this->IsArgumentSet(this->ConfigFilePathArgumentName);
}

std::string CommandLineArguments::GetConfigFilePath()
{
    return this->GetArgumentValue(this->ConfigFilePathArgumentName);
}

void CommandLineArguments::Parse(int argc, char **argv)
{
    CommandLineArgumentCollection::Parse(argc, argv);

    if (!this->IsConfigFilePathSet())
    {
        throw exception::command_line_argument::argument_not_found();
    }
}
