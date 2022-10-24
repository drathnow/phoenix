#include <cstdio>
#include <cctype>
#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <stdexcept>
#include "bbb.h"
#include "BbbGpio.h"
#include <file.h>

namespace dios::bbb
{

using namespace std;
using namespace dios::hal;
using namespace dios::foundation;

BbbGpio::BbbGpio(pin_t pin, const char *name, const char *label) noexcept :
        _pin(pin),
        _name(new char[::strlen(name)+1]),
        _label(new char[::strlen(label)+1]),
        _activeLow(false)
{
    ::strcpy(_name, name);
    ::strcpy(_label, label);

    char buf[64];

    ::sprintf(buf, GPIO_VALUE_FMT, pin.number);
    _valuePath = new char[::strlen(buf)+1];
    ::strcpy(_valuePath, buf);
}

BbbGpio::~BbbGpio()
{
    delete[] _name;
    delete[] _label;
    delete[] _valuePath;
}

const char* BbbGpio::label()
{
    return _label;
}

const char* BbbGpio::name()
{
    return _name;
}

int BbbGpio::value()
{
    char buf[32];
    int value = 0;

    ::sprintf(buf, GPIO_VALUE_FMT, _pin.number);
    FILE* file = ::fopen(buf, "r");
    ::fscanf(file, "%d", &value);
    ::fclose(file);
    return value;
}

void BbbGpio::setValue(int value)
{
    FILE* file = ::fopen(_valuePath, "w");
    ::fprintf(file, "%d", value);
    ::fclose(file);
}

bool BbbGpio::isActiveLow()
{
    int value = 0;

    FILE* file = ::fopen(_valuePath, "r");
    ::fscanf(file, "%d", &value);
    ::fclose(file);
    return value == 1;
}

void BbbGpio::setActiveLow(bool isActiveLow)
{
    FILE* file = ::fopen(_valuePath, "w");
    ::fprintf(file, "%d", isActiveLow ? 1 : 0);
    ::fclose(file);
}

bool BbbGpio::isReadOnly()
{
    return direction() == gpio::out;
}

void BbbGpio::setDirection(Direction direction)
{
    FILE* file = ::fopen(_valuePath, "w");
    ::fprintf(file, "%s", direction == Direction::in ? "in" : "out");
    ::fclose(file);
}

dios::hal::gpio::Direction BbbGpio::direction() const
{
    char buf[8];

    FILE* file = ::fopen(_valuePath, "r");
    ::fscanf(file, "%s", buf);
    ::fclose(file);
    return ::strcmp("in", buf) == 0 ? gpio::Direction::in : gpio::Direction::out;
}

static void verifyPaths(const char* pathFormat, int gpioNumber)
{
    char buf[64];

    ::sprintf(buf, pathFormat, gpioNumber);
    if (!File::exists(buf))
        throw runtime_error("File not found: " + string(buf));
}

static BbbGpio* BbbGpio::createGpio(pin_t pin, const char* name, const char* label)
{
    struct stat file_stat;

    ::memset(&file_stat, 0, sizeof(struct stat));
    ::sscanf(name, "GPIO_%hu", (uint16_t*)&pin.number);

    if (!File::exists(GPIO_DIR_PATH))
        throw runtime_error("File not found: " + string(GPIO_DIR_PATH));

    if (!File::exists(GPIO_EXPORT_PATH))
        throw runtime_error("File not found: " + string(GPIO_EXPORT_PATH));

    FILE* f = ::fopen(GPIO_EXPORT_PATH, "w");
    ::fprintf(f, "%d", pin.number);
    ::fclose(f);

    verifyPaths(GPIO_PATH_FMT, pin.number);
    verifyPaths(GPIO_VALUE_FMT, pin.number);
    verifyPaths(GPIO_ACTIVE_LOW_FMT, pin.number);
    verifyPaths(GPIO_DIRECTION_FMT, pin.number);

    return new BbbGpio(pin, name, label);
}

} /* namespace dios */
