//
// Created by anowikowski on 17.12.15.
//

#include "CommandLineArgument.h"

CommandLineArgument::CommandLineArgument(std::string sArgumentName, std::string sArgumentValue)
{
    this->SetArgumentName(sArgumentName);
    this->SetArgumentValue(sArgumentValue);
}

std::string CommandLineArgument::GetArgumentName()
{
    return this->m_sArgumentName;
}

void CommandLineArgument::SetArgumentName(const std::string &sArgumentName)
{
    this->m_sArgumentName = sArgumentName;
}

std::string CommandLineArgument::GetArgumentValue()
{
    return this->m_sArgumentValue;
}

void CommandLineArgument::SetArgumentValue(const std::string &sArgumentValue)
{
    this->m_sArgumentValue = sArgumentValue;
}
