/** @file NamedType.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jul 09, 2013 by daver:
 * @endhistory
 */

#ifndef NAMEDTYPE_H_
#define NAMEDTYPE_H_

#include <stdint.h>
#include <string>

namespace zios {
namespace common {

class NamedType {
public:
    NamedType(const NamedType& namedType);
    NamedType(const char* name, int number);
	NamedType(const std::string& name, int number);
	virtual ~NamedType();

	inline const std::string& name() const { return _name; }
	inline const int number() const { return _number; }
	const std::string toString() const;

	NamedType& operator=(const NamedType& otherNamedType);

private:
    std::string _name;
    uint16_t _number;

};

} /* namespace zap */
} /* namespace zios */

#endif /* NAMEDTYPE_H_ */
