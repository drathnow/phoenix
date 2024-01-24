// Copyright 2009 Micha�l Larouche <larouche@kde.org>
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
#ifndef _CPPINJECT_GLOBALZONETYPE_H_
#define _CPPINJECT_GLOBALZONETYPE_H_

#include "ZoneType.h"

namespace CppInject
{

class GlobalZoneType : public ZoneType
{
public:
	static const GlobalZoneType instance;

private:
	GlobalZoneType() {}
};

}
#endif
