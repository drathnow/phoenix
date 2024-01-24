/** @file StringUtl.cpp
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Oct 29, 2014 by eganb:
 * IncrDev Dec 10, 2015 by eb:    add hexStringToUint fnc
 * @endhistory
 */


#include <cstdio>
#include <cerrno>
#include <climits>
#include <iostream>  // cerr
#include <stdlib.h>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <sstream>
#include <iostream>
#include "StringUtl.h"

using namespace std;
namespace zios{
namespace common{

StringUtl::StringUtl() {
}

StringUtl::~StringUtl() {
}

/*******
 *
 * helper function to get unsigned bool from string
 */
bool StringUtl::stringToBool(string input, bool& boolOut){
    int32_t number;
    float fNumber;
    bool rc = false;
    if (stringToInt(input, number)){
        // was a number so simple conversion...
        rc = true;
        if (number != 0)
            boolOut = true;
        else
            boolOut = false;
    } else {
        if (stringToFloat(input, fNumber)) {
            // was a number so simple conversion...
            rc = true;
            if (fNumber != 0)
                boolOut = true;
            else
                boolOut = false;
        } else {
            trim(input);            /// remove spaces from infront and behind
            if ((icompare("true", input)) || (icompare("on", input))) {
                rc = true;
                boolOut = true;

            } else if ((icompare("false", input)) || (icompare("off", input))) {
                rc = true;
                boolOut = false;
            }
        } // end else was float
        // else couldn't convert.... return false....
    }
    return rc;
}

/*******
 *
 * helper function to get unsigned integer from string
 */
bool StringUtl::stringToUint(string input, uint32_t& intOut) {
    bool rc = false;
    //cerr << "input: " << input << endl;
    trim(input);            /// remove spaces from infront and behind

    if (input.find_first_not_of("+0123456789") == std::string::npos) {
        rc = true;  // assume we are ok till failure
        if (input.find_first_of("+") != std::string::npos) {
            // found a + somewhere in string....
//            size_t one = input.find_first_of("+");
//            size_t two = input.find_last_of("+");
//            cerr << "one two : " << one <<" " << two << endl;
            if ((input.find_first_of("+") != input.find_last_of("+")) || (0 != input.find_last_of("+"))) {
             // more than one + or - or not at beginning
                rc = false;
            }
        }
        if (rc == true){
            uint32_t number = strtoul(input.c_str(), NULL, 10);
            if (ULONG_MAX == number && ERANGE == errno)
                rc = false;
            else
                intOut = number;
        }
    }
    return rc;
}

bool StringUtl::stringToU64(std::string input, uint64_t& longOut) {
    bool rc = false;
    trim(input);            /// remove spaces from infront and behind

    if (input.find_first_not_of("+0123456789") == std::string::npos) {
        rc = true;  // assume we are ok till failure
        if (input.find_first_of("+") != std::string::npos) {
            if ((input.find_first_of("+") != input.find_last_of("+")) || (0 != input.find_last_of("+"))) {
                rc = false;
            }
        }
        if (rc == true){
            uint64_t number = ::strtoull(input.c_str(), NULL, 10);
            if (ULONG_MAX == number && ERANGE == errno)
                rc = false;
            else
                longOut = number;
        }
    }
    return rc;
}



/*******
 *
 * helper function to get hex integer from string
 */
bool StringUtl::hexStringToUint(string input, uint32_t& intOut) {
    bool rc = false;
    //cerr << "input: " << input << endl;
    trim(input);            /// remove spaces from infront and behind

    if (input.find_first_not_of("x0123456789ABCDEFabcdef") == std::string::npos) {
        rc = true;  // assume we are ok till failure
        if (input.find_first_of("x") != std::string::npos) {
            // found a + somewhere in string....
            if ((input.find_first_of("x") != input.find_last_of("x")) || (input.find_last_of("x") != 1) || (input[0]!= '0')) {
             // more than one + or - or not at 2 char, i.e. x not 0x
                rc = false;
            }
        }
        if (rc == true){
            uint32_t number = strtoul(input.c_str(), NULL, 16);
            if (ULONG_MAX == number && ERANGE == errno)
                rc = false;
            else
                intOut = number;
        }
    }
    return rc;
}


/*******
 *
 * helper function to get signed integer from string
 */
bool StringUtl::stringToInt(string input, int32_t& intOut){
    bool rc = false;
    trim(input);            /// remove spaces from infront and behind

    if( input.find_first_not_of("+-0123456789") == std::string::npos){
        // couldn't find anything non "numberish"....
        rc = true;  // assume we are ok till failure
        if (input.find_first_of("+-") != std::string::npos) {
            // found a + or minus somewhere in string....
//            size_t one = input.find_first_of("+-");
//            size_t two = input.find_last_of("+-");
            //cerr << "one two : " << one <<" " << two << endl;
            if ((input.find_first_of("+-") != input.find_last_of("+-")) || (0 != input.find_last_of("+-"))) {
             // more than one + or - or not at beginning
                rc = false;
            }
        }
        if (rc == true){
        //  .. only 1 + or minus in string
            int32_t number = strtol(input.c_str(), NULL, 10);
            //int ec = errno;         // because reading it changes it...
            if ((LONG_MAX == number || LONG_MIN == number) && ERANGE == errno)
                rc = false;
            else
                intOut = number;
        }
    } // if number
    return rc;
}

/*******
 *
 * helper function to get float from string
 */
bool StringUtl::stringToFloat(string input, float& floatOut) {
    bool rc = false;
    double number;
    trim(input);            /// remove spaces from infront and behind
    if (input.find_first_not_of(".+-0123456789") == std::string::npos) {
        // couldn't find anything non numberish....
        rc = true;            // unless something is wrong then assume we can convert....
        if (input.find_first_of("+-") != std::string::npos) {
            // found a + or minus somewhere in string....
            if ((input.find_first_of("+-") != input.find_last_of("+-")) || (0 != input.find_last_of("+-"))) {
             // more than one + or - or not at beginning
                rc = false;
            }
        }
        if (input.find_first_of(".") != std::string::npos) {
            if (input.find_first_of(".") != input.find_last_of(".")) {
                // more than one .
                rc = false;
            }
        }
        if (rc) {
            number = ::strtod(input.c_str(), NULL);
            floatOut = (float) number;
        }
    } else if (input.find("e-") != string::npos || input.find("E-") != string::npos) {
        rc = (floatOut = ::atof(input.c_str())) != 0.0;
    }
    return rc;
}

char **StringUtl::createStringArray(std::vector<std::string> &input) {
    char **output = NULL;

    // need space for each entry in input, plus the null at the end
    // calloc sets everything to null, so we don't need to set the last entry to NULL ourselves
    int len = input.size();
    output = (char **) calloc(len+1, sizeof(char*));

    if (output == NULL) {
        return NULL;
    }

    for (int i = 0; i < len; i++) {
        output[i] = strdup(input[i].c_str());
    }

    return output;

}

void StringUtl::freeStringArray(char **strArray) {
    if (strArray == NULL) {
        return;
    }

    for (char **p = strArray; *p != NULL; p++) {
        free(*p);
    }
    free(strArray);
}

string StringUtl::intToString(int32_t input) {
    stringstream ss;
    ss << input;
    return ss.str();
}

string StringUtl::uintToString(uint32_t input) {
    stringstream ss;
    ss << input;
    return ss.str();
}

std::map<std::string, std::string>* StringUtl::stringToMap(const std::string& input, char delim, bool ignoreComments) {
    map<string, string>* pairs = new map<string, string>();
    stringToMap(input, *pairs, delim, ignoreComments);
    return pairs;
}

void StringUtl::stringToMap(const std::string& input, std::map<std::string, std::string>& retMap, char delim, bool ignoreComments) {
    stringstream ss(input);
    streamToMap(ss, retMap, delim, ignoreComments);
}

int StringUtl::streamToMap(istream& inputStream, std::map<std::string, std::string>& retMap, char delim, bool ignoreComments)
{
    char buf[256];
    string line;
    string key;
    string value;

    // for each line found
    while (inputStream.getline(buf, sizeof(buf))) {
        // ignore comment lines if requested :)
        if (buf[0] != '#' || !ignoreComments) {
            string line(buf);
            if (stringToKeyValue(line, key, value, delim))
                retMap.insert(pair<string, string>(key, value));
        }
    }
    return retMap.size();
}


bool StringUtl::stringToKeyValue(const std::string& keyValueString, std::string& retKey, std::string& retValue, char delim)
{
    size_t equalsPos = keyValueString.find(delim);
    if (string::npos != equalsPos) {
        retKey = keyValueString.substr(0, equalsPos);
        retValue = keyValueString.substr(equalsPos + 1, keyValueString.length());
        StringUtl::trim(retKey);
        StringUtl::trim(retValue);
        return true;
    }
    return false;
}


vector<string>* StringUtl::stringToVector(const string& line, char delim, bool ignoreComments) {
    vector<string> * result = new vector<string>;
    stringToVector(line, *result, delim, ignoreComments);
    return result;
}



char* StringUtl::trimString(char *str) {
    char *end;

    while (isspace((unsigned char)*str) || *str == '\t')
        str++;

    if (*str == 0)  // All spaces?
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    *(end + 1) = 0;
    return str;
}

void StringUtl::stringToVector(const string& str, vector<string>& retVector, char delim, bool ignoreComments) {
    string line(str);
    StringUtl::trim(line);
    // # is a comment
    if ((line.empty() || (ignoreComments && (line[0] == '#'))) == false) {
        size_t start=0, delimPos;
        string fragment;
        while (start != string::npos) {
            delimPos = line.find_first_of(delim,start);
            // no more delimiters, use from start to end of string
            if (delimPos == string::npos) {
                fragment = line.substr(start);
                start = string::npos;
            } else {
                fragment = line.substr(start, delimPos-start);
                start = delimPos + 1;   // start at next char after delim
                // we found a delim, so even if there are no more chars,
                // repeat the loop one more time to pick up the empty string
                if (start > line.length()) {
                    start = line.length();
                }
            }
            StringUtl::trim(fragment);
            retVector.push_back(fragment);
        }
    }
}


}
}// namespace

