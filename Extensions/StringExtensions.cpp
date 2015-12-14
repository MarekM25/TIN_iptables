//
// Created by anowikowski on 13.12.15.
//

#include "StringExtensions.h"

#include <algorithm>
#include <stdexcept>

namespace string_extensions
{
    bool ends_with(const std::string &str, const std::string &ending) {
        if (str.length() >= ending.length()) {
            return (str.compare(str.length() - ending.length(), ending.length(), ending) == 0);
        }

        return false;
    }

    unsigned int stoui(const std::string &s)
    {
        unsigned long lresult = stoul(s, 0, 10);
        unsigned int iresult = lresult;
        if (lresult != (unsigned long)iresult)
        {
            throw std::out_of_range("");
        }

        return iresult;
    }

    std::string tolower(const std::string &str)
    {
        std::string lowerStr = str;
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
        return lowerStr;
    }
}

