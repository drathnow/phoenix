// Copyright 2009 Micha�l Larouche <larouche@kde.org>
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include "Zone.h"

#include <cassert>
#include <iterator>
#include <new>
#include <vector>

#include "destroyer.h"
#include "ZoneType.h"

namespace CppInject {

Zone* Zone::_globalZone = NULL;

Zone::Zone(const ZoneType* type) :
        _zoneType(type), _parent(0) {
}

Zone::Zone(const ZoneType* type, Zone * parent) :
        _zoneType(type), _parent(parent) {
}

void Zone::initGlobalZone(Zone* globalZone) {
    assert(_globalZone == NULL); // You can only call this once!
    _globalZone = globalZone;
}

void Zone::destroyGlobalZone() {
    delete _globalZone;
}

Zone& Zone::globalZone() {
    assert(_globalZone != NULL); // You must set the global zone
    return *_globalZone;
}

Zone::~Zone() {
    for (std::vector<destroyer*>::iterator itr = _destroyers.begin(); itr != _destroyers.end(); ++itr) {
        destroyer* destroy = *itr;
        (*destroy)();
        delete destroy;
    }
}

bool Zone::operator==(const Zone &other) {
    return _zoneType == other._zoneType;
}

void Zone::push_destroyer(destroyer* d) {
    _destroyers.push_back(d);
}

Zone *Zone::parent() const {
    return _parent;
}

const ZoneType *Zone::zoneType() const {
    return _zoneType;
}

}
