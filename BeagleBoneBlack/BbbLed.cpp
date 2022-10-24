#include <cstdio>
#include <cctype>
#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <stdexcept>
#include "bbb.h"
#include "BbbLed.h"
#include <file.h>

namespace dios
{
namespace foundation
{

BbbLed::BbbLed(int number, const char* name) :
        _number(number),
        _name(new char[::strlen(name)+1]),
        _brightnessPath(nullptr)
{
    ::strcpy(_name, name);

    char buf[64];

    ::sprintf(buf, GPIO_VALUE_FMT, number);
    _brightnessPath = new char[::strlen(buf)+1];
    ::strcpy(_brightnessPath, buf);
}

BbbLed::~BbbLed()
{
    delete[] _brightnessPath;
}

void BbbLed::turnOff()
{
    FILE* file = ::fopen(_brightnessPath, "w");
    ::fprintf(file, "%d", 0);
    ::fclose(file);
}

void BbbLed::turnOn()
{
    FILE* file = ::fopen(_brightnessPath, "w");
    ::fprintf(file, "%d", 1);
    ::fclose(file);
}

void BbbLed::setFlash(int freqencyMilliseconds, int durationMilliseconds)
{

}

} /* namespace foundation */
} /* namespace dios */
