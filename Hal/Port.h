#ifndef __PORT_H___
#define __PORT_H___

class Port
{
public:
    virtual ~Port() {}

    virtual int read(char& buffer, int bufferSize) = 0;
    virtual int write(const char& data, int dataLength) = 0;
    virtual void close() = 0;
};

#endif /* __PORT_H___ */
