#include <File.h>
#include <iostream>
#include <Random.h>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <UnsupportedOperationException.h>

#include "BlobValue.h"

namespace zios::domain {

using namespace std;
using namespace zios::common;

static void appendBufferToFile(const uint8_t *buffer, uint32_t size, File &file)
{
    fstream fs;
    fs.open(file.absolutePath(), ios_base::binary | ios_base::app | ios_base::out);
    fs.write((const char*) buffer, size);
    fs.close();
}

static File* tempFileContainingBuffer(const uint8_t *buffer, uint32_t size)
{
    Random random;
    stringstream ss;
    ss << random.nextU32();
    string filename = File::createTempFilename(ss.str().c_str(), "blob");
    File *file = new File(filename);
    if (buffer != NULL && size > 0)
    {
        appendBufferToFile(buffer, size, *file);
    }
    return file;
}

BlobValue::BlobValue() :
        _file(NULL), _value(NULL), _size(0)
{
}

BlobValue::BlobValue(const uint8_t *value, uint32_t size)
{
    if (MAX_MEMORY_SIZE >= size)
    {
        _file = NULL;
        _value = new uint8_t[size];
        ::memcpy(_value, value, size);
    }
    else
    {
        _file = tempFileContainingBuffer(value, size);
        _value = NULL;
    }
    _size = size;
}

BlobValue::BlobValue(const File &file) :
        _value(NULL), _size(0)
{
    _file = new File(file);
}

uint32_t BlobValue::size() const
{
    return _size;
}


BlobValue::~BlobValue()
{
    if (_file == NULL)
    {
        delete[] _value;
        _value = NULL;
        _size = 0;
    }
    else
    {
        _file->turf();
        delete _file;
    }
}

BlobValue::BlobValue(BlobValue &&otherValue)
{
    if (_file == NULL)
    {
        _value = otherValue._value;
        _size = otherValue._size;
    }
    else
    {
        _file = otherValue._file;
        otherValue._file = NULL;
    }
}

uint32_t BlobValue::append(const uint8_t *appendValue, uint32_t appendSize)
{
    if (_file == NULL)
    {
        if (_size + appendSize > MAX_MEMORY_SIZE)
        {
            _file = tempFileContainingBuffer(_value, _size);
            appendBufferToFile(appendValue, appendSize, *_file);
            delete[] _value;
            _value = NULL;
        }
        else
        {
            uint32_t newSize = appendSize + _size;
            const uint8_t *tmp = _value;
            _value = new uint8_t[newSize];
            ::memcpy(_value, tmp, _size);
            ::memcpy(&_value[_size], appendValue, appendSize);
            delete[] tmp;
        }
    }
    else
    {
        appendBufferToFile(appendValue, appendSize, *_file);
    }

    _size += appendSize;
    return _size;
}

ByteBuffer& BlobValue::deserialize(ByteBuffer &byteBuffer)
{
    uint32_t inputSize = byteBuffer.getU32();
    if (inputSize >= MAX_MEMORY_SIZE)
    {
        _file = tempFileContainingBuffer(_value, _size);
        delete[] _value;
        _size = 0;
    }

    uint8_t buffer[MAX_MEMORY_SIZE];
    while (inputSize > 0)
    {
        int readSize = byteBuffer.get((int8_t*) buffer, sizeof(buffer));
        append(_value, readSize);
        inputSize -= readSize;
    }

    return byteBuffer;
}

ByteBuffer& BlobValue::serialize(ByteBuffer &byteBuffer) const
{
    if (_file == NULL)
    {
        byteBuffer.putU32(_size);
        byteBuffer.put((const int8_t*) _value, _size);
    }
    else
    {
        int size = _file->size();
        if (size > 0)
        {
            byteBuffer.putU32((uint32_t) _size);
            uint8_t buffer[2048];
            fstream fs(_file->absolutePath());
            while (size > 0)
            {
                fs.read((char*) buffer, sizeof(buffer));
                byteBuffer.put((const int8_t*) buffer, fs.gcount());
                size -= fs.gcount();
            }
            fs.close();
        }
    }
    return byteBuffer;
}

BlobValue::operator uint8_t() const { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
BlobValue::operator int8_t() const { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
BlobValue::operator uint16_t() const  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
BlobValue::operator int16_t() const  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
BlobValue::operator uint32_t() const  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
BlobValue::operator int32_t() const  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
BlobValue::operator uint64_t() const  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
BlobValue::operator int64_t() const  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
BlobValue::operator float() const  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
BlobValue::operator double() const  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
BlobValue::operator std::string() const  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }

bool BlobValue::assign(uint8_t newValue)  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
bool BlobValue::assign(int8_t newValue)  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
bool BlobValue::assign(uint16_t newValue)  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
bool BlobValue::assign(int16_t newValue)  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
bool BlobValue::assign(uint32_t newValue)  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
bool BlobValue::assign(int32_t newValue)  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
bool BlobValue::assign(uint64_t newValue)  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
bool BlobValue::assign(int64_t newValue)  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
bool BlobValue::assign(float newValue)  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
bool BlobValue::assign(double newValue)  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
bool BlobValue::assign(const std::string& newValue)  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
bool BlobValue::assign(std::string&& newValue)  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }

Value& BlobValue::operator=(const Value& otherValue)  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }

bool BlobValue::operator==(const Value& otherValue) const  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
bool BlobValue::operator<(const Value& otherValue) const  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
bool BlobValue::operator>(const Value& otherValue) const  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
bool BlobValue::operator<=(const Value& otherValue) const  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }
bool BlobValue::operator>=(const Value& otherValue) const  { THROW_NAMED_EXCEPTION(UnsupportedOperationException, "Operation not supported"); }

} /* namespace zios */
