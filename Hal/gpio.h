#ifndef __GPIO_H___
#define __GPIO_H___

#include <cstdint>

namespace dios
{

namespace hal
{

    class gpio
    {
    public:

        enum Direction
        {
            in, out
        };

        virtual ~gpio() = default;

        virtual const char* label() = 0;

        virtual const char* name() = 0;

        virtual int32_t value() = 0;

        virtual void setValue(int value) = 0;

        virtual bool isActiveLow() = 0;

        virtual void setActiveLow(bool isActiveLow) = 0;

        virtual bool isReadOnly() = 0;

        virtual void setDirection(Direction direction);

        virtual Direction direction() const = 0;
    };
}
}

#endif /* __GPIO_H___ */
