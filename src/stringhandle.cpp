/* ****************************************************************
 * FilePath     : /src/stringhandle.cpp
 * Project Name : CGMerge
 * Author       : zzyy21
 * Create Time  : 2020-06-23 18:54:21
 * Modifed by   : zzyy21
 * Last Modify  : 2020-06-23 18:54:21
 * Description  : remove eol char from end of string 
 * Revision     : 
 * **************************************************************** */

#include "stringhandle.h"

#include <string>

void removeEOLChar(std::string *inString) {
    int size = (*inString).size();
    char endChar = (*inString).at(size - 1);
    while ((endChar == '\r') || (endChar == '\n')) {
        (*inString).erase(size - 1, 1);
        size--;
        endChar = (*inString).at(size - 1);
    }
}
