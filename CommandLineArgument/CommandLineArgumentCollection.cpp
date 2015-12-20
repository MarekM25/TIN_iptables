//
// Created by anowikowski on 17.12.15.
//

#include "CommandLineArgumentCollection.h"
#include "../Extensions/StringExtensions.h"
#include "../Exception/CommandLineArgumentException.h"

const std::string CommandLineArgumentCollection::m_sArgumentNameValueSeparator = "=";
const std::string CommandLineArgumentCollection::m_sArgumentPrefix = "-";

CommandLineArgumentCollection::CommandLineArgumentCollection()
{

}

bool CommandLineArgumentCollection::IsArgumentSet(std::string sArgumentName)
{
    std::string sArgumentNameLowerCase = string_extensions::tolower(sArgumentName);

    for (std::vector<CommandLineArgument>::iterator it = this->m_arguments.begin(); it != this->m_arguments.end(); it++ )
    {
        std::string sCurrentArgumentNameLowerCase = string_extensions::tolower(it->GetArgumentName());
        if (sArgumentNameLowerCase == sCurrentArgumentNameLowerCase)
        {
            return true;
        }
    }

    return false;
}

std::string CommandLineArgumentCollection::GetArgumentValue(std::string sArgumentName)
{
    std::string sArgumentNameLowerCase = string_extensions::tolower(sArgumentName);

    for (std::vector<CommandLineArgument>::iterator it = this->m_arguments.begin(); it != this->m_arguments.end(); it++ )
    {
        std::string sCurrentArgumentNameLowerCase = string_extensions::tolower(it->GetArgumentName());
        if (sArgumentNameLowerCase == sCurrentArgumentNameLowerCase)
        {
            return it->GetArgumentValue();
        }
    }

    throw exception::command_line_argument::argument_not_found();
}

void CommandLineArgumentCollection::Parse(int argc, char *argv[])
{
    this->Clear();

    for (int i = 1; i < argc; ++i)
    {
        std::string sRawArgument = std::string(argv[i]);
        size_t indexOfSeparator = sRawArgument.find(this->m_sArgumentNameValueSeparator);
        if (indexOfSeparator == std::string::npos)
        {
            throw exception::command_line_argument::invalid_argument_format();
        }

        std::string argumentName = sRawArgument.substr(0, indexOfSeparator);
        std::string argumentValue = sRawArgument.substr(indexOfSeparator + this->m_sArgumentNameValueSeparator.length());

        if (argumentName.substr(0, this->m_sArgumentPrefix.length()) != this->m_sArgumentPrefix)
        {
            throw exception::command_line_argument::invalid_argument_format();
        }

        argumentName = argumentName.substr(this->m_sArgumentPrefix.length());

        if (this->IsArgumentSet(argumentName))
        {
            throw exception::command_line_argument::argument_already_present();
        }

        this->m_arguments.push_back(CommandLineArgument(argumentName, argumentValue));
    }
}

void CommandLineArgumentCollection::Clear()
{
    this->m_arguments = std::vector<CommandLineArgument>();
}
