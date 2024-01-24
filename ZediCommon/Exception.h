/** @file Exception.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jul 29, 2013 by daver:
 * @endhistory
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_
#include <string>
#include <exception>
#include <sstream>

namespace zios {
namespace common {

/**
 * Use this macro to construct and throw a specific exception with a message.  This macro will
 * build an Exception object passing in the function name, line number, and message. This macro
 * assumes that the named exception is a subclass of Exception and that it has a constructor that
 * will pass the necessary argument to the base Exception class.
 */
#define THROW_NAMED_EXCEPTION(namedException, message){      \
        std::ostringstream oss;                              \
        oss << message;                                      \
        throw(namedException(__func__, __LINE__, oss.str()));\
    }


/**
 * Will throw an exception, with a message, if the contition
 * evaluates to true.
 */
#define THROW_EXCEPTION_IF(condition, message){                     \
        if (condition) {                                            \
            std::ostringstream oss;                                 \
            oss << message;                                         \
            throw(Exception(__func__, __LINE__, oss.str()));        \
        }                                                           \
    }

/**
 * Use this macro to construct and throw an Exception with a message.  This macro will
 * build an Exception object passing in the function name, line number, and message.
 */
#define THROW_EXCEPTION(message)   {                    \
        std::ostringstream oss;                         \
        oss << message;                                 \
        throw(Exception(__func__, __LINE__, oss.str()));\
    }

class Exception : public std::exception {

public:
    Exception(const char* funcName, int lineNumber, const std::string& message);
	Exception(const Exception& exception);
	virtual ~Exception() throw();

	Exception& operator=(const Exception& otherException);

	virtual const char* what() const throw();
	virtual inline const std::string& message() const { return _message; }

protected:
    Exception(const char* funcName, int lineNumber, const char* name, const std::string& message);
    Exception(const std::string& message);

	std::string _message;
};

}/* namespace common */
} /* namespace zios */
#endif /* EXCEPTION_H_ */
