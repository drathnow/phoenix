#pragma once

#ifndef __STRING_H___
#define __STRING_H___

#include <cstring>
#include <iostream>

using namespace std;

class String
{
public:
    String() :
            _str(nullptr)
    {
        cout << "Con called" << endl;
    }

    String(const char *str)
    {
        cout << "String con called" << endl;
        _str = new char[::strlen(str) + 1];
        ::strcpy(_str, str);
    }

    String(const String& str)
    {
        cout << "Copy con called" << endl;
        _str = new char[::strlen(str._str) + 1];
        ::strcpy(_str, str._str);
    }

    String(String&& str)
    {
        cout << "Move con called" << endl;
        _str = str._str;
        str._str = nullptr;
    }

    ~String()
    {
        delete[] _str;
    }

    String& operator=(const String &s)
    {
        cout << "Copy assign called" << endl;
        _str = new char[::strlen(s._str) + 1];
        ::strcpy(_str, s._str);
        return *this;
    }

    String& operator=(String&& str)
    {
        cout << "Move assign called" << endl;
        _str = str._str;
        str._str = nullptr;
        return *this;
    }

    friend ostream& operator <<(ostream &out, const String &s);

    char *_str;
};

#endif /* __STRING_H___ */
