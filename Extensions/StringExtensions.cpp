//
// Created by anowikowski on 13.12.15.
//

#include "StringExtensions.h"

#include <algorithm>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <stdexcept>

namespace string_extensions
{
    bool ends_with(const std::string &str, const std::string &ending) {
        return ending.length() <= str.length() &&
               str.compare(str.length() - ending.length(), ending.length(), ending) == 0;
    }

    unsigned int stoui(const std::string &s)
    {
        unsigned long lresult = stoul(s, 0, 10);
        unsigned int iresult = (unsigned int)lresult;
        if (lresult != (unsigned long)iresult)
        {
            throw std::out_of_range("");
        }

        return iresult;
    }

    unsigned short stous(const std::string &s)
    {
        unsigned long lresult = stoul(s, 0, 10);
        unsigned short iresult = (unsigned short)lresult;
        if (lresult != (unsigned long)iresult)
        {
            throw std::out_of_range("");
        }

        return iresult;
    }

    std::string trim( const std::string& sourceString )
    {
        std::string delimiters = " \t\r\n";
        std::string result( sourceString );

        auto index = result.find_last_not_of( delimiters );
        if( index != std::string::npos )
            result.erase( ++index );

        index = result.find_first_not_of( delimiters );
        if( index != std::string::npos )
            result.erase( 0, index );
        else
            result.erase();

        return result;
    }

    std::string tolower(const std::string &str)
    {
        std::string lowerStr = str;
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
        return lowerStr;
    }

    std::vector<std::string> split(const std::string &s) {
        std::vector<std::string> tokens;
        std::istringstream iss(s);
        std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(tokens));
        return tokens;
    }
}

