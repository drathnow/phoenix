/** @file StringUtl.h
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
#ifndef STRINGUTL_H_
#define STRINGUTL_H_


#include <stdint.h>
#include <map>
#include <algorithm>
#include <string>
#include <vector>
#include <cstring>

namespace zios {
namespace common {

/**
 * NOTE: THESE ROUTINES DO NOT PRESERVE INPUT STRING!!!!
 */

class StringUtl {
public:
    ~StringUtl();

    // trim from start
    static inline std::string& ltrim(std::string& s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
            return s;
    }

    // trim from end
    static inline std::string& rtrim(std::string& s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
            return s;
    }

    // trim from both ends
    static inline std::string& trim(std::string& s) {
            return ltrim(rtrim(s));
    }

    // used for case insensitive string compare
    static inline bool icompare(const std::string& a, const std::string& b){
        bool rc = ( 0  == strcasecmp(a.c_str(),b.c_str()));
    //    std::cerr << "chk2: " << rc << " " << a << "," << b << std::endl;
        return rc;
    }

    static bool stringToBool(std::string input, bool& boolOut);

    /**
     * Trims leading and trailing spaces and tabs
     *
     * @param str
     * @return A trimmer string
     */
    static char* trimString(char *str);


    /*******
     *
     * helper function to get unsigned integer from hex string
     */
    static bool hexStringToUint(std::string input, uint32_t& intOut);

    /*******
     *
     * helper function to get unsigned integer from string
     */
    static bool stringToUint(std::string input, uint32_t& intOut);

    /*******
     *
     * helper function to get unsigned 64 bit int from string
     */
    static bool stringToU64(std::string input, uint64_t& longOut);

    /*******
     *
     * helper function to get signed integer from string
     */
    static bool stringToInt(std::string input, int32_t& intOut);

    /*******
     *
     * helper function to get float from string
     */
    static bool stringToFloat(std::string input, float& floatOut);

    /**
     * Create an array of strings from string arguments.
     * The output array is null terminated, in that the last entry will hold a NULL
     *
     * @param input vector holding strings.  vector may be empty
     * @return a pointer to an array of strings with NULL termination
     */
    static char **createStringArray(std::vector<std::string> &input);

    /**
     * Frees the memory used by a 2 dimensional array of strings.
     * This will free all strings held in the array, and the array itself.
     * Assumes all memory was allocated using "malloc" or equivalent
     *
     * @param strArray  array of char pointers.
     */
    static void freeStringArray(char **strArray);

    /**
     * Converts an integer to a string
     * @param input integer value
     * @return string representation using standard formatting
     */
    static std::string intToString(int32_t input);

    /**
     * Converts an unsigned integer to a string
     * @param input unsigned integer value
     * @return string representation using standard formatting
     */
    static std::string uintToString(uint32_t input);

    /**
     * Convert a string into a map.
     * Each line must end with a NL
     * The key/value delimiter is supplied as an option
     * Spaces around the key or value are trimmed
     * The caller must destroy the returned value.
     *
     * @param[in] input string to convert
     * @param[in] delim the character to split the key/value pair, defaults to =
     * @param[in] ignoreComments - if true, ignores lines with # or empty lines
     * @return pointer to a map.
     */
    static std::map<std::string, std::string>* stringToMap(const std::string& input, char delim = '=',bool ignoreComments=true);

    /**
     * Convert a string into a map, returning the results in a map provided by the caller
     * Each line must end with a NL
     * The key/value delimiter is supplied as an option
     * Spaces around the key or value are trimmed
     * The caller must destroy the returned value.
     *
     * @param[in] input string to convert
     *
     * @param[in] retMap, the map to return values in. Contents of the map are cleared before parsing
     * begins so if the caller need a clear map, he must clear it himself.
     *
     * @param[in] delim the character to split the key/value pair, defaults to =
     *
     * @param[in] ignoreComments - if true, ignores lines with # or empty lines
     *
     * @return pointer to a map.
     */
    static void stringToMap(const std::string& input, std::map<std::string, std::string>& retMap, char delim = '=',bool ignoreComments=true);

    /**
     * Converts a stream into a map supplied by the caller.  The stream should contain individual lines delimited by newline characters.  Each
     * line should have the formate <key><delim><value>.  The default delimiter is an equal (=) sign but it can be whatever is specified by
     * the delim argument.
     *
     * @param stringStream - The input stream contain chars
     *
     * @param retMap - the map to return key/value pairs.
     *
     * @param delim - The delimiter between key/value (default is '=')
     *
     * @param ignoreComments - Set to true to ignore lines starting with '#' character
     *
     * @return The number of key/value pairs parsed from the stream
     */
    static int streamToMap(std::istream& inputStream, std::map<std::string, std::string>& retMap, char delim = '=', bool ignoreComments = true);

    /**
     * Separates a delimited key/value string into its component parts.
     *
     * @param keyValueString - A string containing <key><delim><value>.
     *
     * @param retKey - Place to return the key
     *
     * @param retValue - Place to return the value
     *
     * @return true if the string was parsed successfully, false if the delimiter was not found in the string
     */
    static bool stringToKeyValue(const std::string& keyValueString, std::string& retKey, std::string& retValue, char delim = '=');

    /**
     * Convert a delimited string into a vector.
     * Processing rules:
     *   - whitespace around the entire string is ignored
     *   - if line starts with #, an empty vector is returned
     *   - breaks string into fragments
     *   - each fragment is trimmed to remove white space
     *   - each fragment is added to the vector
     *
     * The caller must destroy the returned value
     *
     * @param[in] line - input line
     * @param[in] delim - the character to split the string on
     * @param[in] ignoreComments - if true, ignores lines with # or empty lines
     * @return pointer to list (may be empty)
     */
    static std::vector<std::string>* stringToVector(const std::string& line, char delim = ',', bool ignoreComments = true);

    /**
     * Convert a delimited string into a vector.
     * Processing rules:
     *   - whitespace around the entire string is ignored
     *   - if line starts with #, an empty vector is returned
     *   - breaks string into fragments
     *   - each fragment is trimmed to remove white space
     *   - each fragment is added to the vector
     *
     * The caller must destroy the returned value
     *
     * @param[in] line - input line
     *
     * @param[in] retVector - Vector to return values.  The vector is not cleared by this method.
     *
     * @param[in] delim - the character to split the string on
     *
     * @param[in] ignoreComments - if true, ignores lines with # or empty lines
     *
     * @return pointer to list (may be empty)
     */
    static void stringToVector(const std::string& line, std::vector<std::string>& retVector, char delim = ',', bool ignoreComments = true);

private:
    StringUtl();
};


}} // namespace


#endif /* STRINGUTL_H_ */
