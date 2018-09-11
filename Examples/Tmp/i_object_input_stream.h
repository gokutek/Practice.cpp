#ifndef I_OBJECT_INPUT_STREAM_H
#define I_OBJECT_INPUT_STREAM_H

#include <string>
#include <stdint.h>

class IObjectInputStream
{
public:
    virtual ~IObjectInputStream() {}

    // Reads an 8 bit byte.
    virtual int8_t ReadInt8() = 0;

    // Reads a 16 bit short.
    virtual int16_t ReadInt16() = 0;

    // Reads a 32 bit int.
    virtual int32_t ReadInt32() = 0;

    // Reads a 64 bit long.
    virtual int64_t ReadInt64() = 0;

    // Reads a 32 bit float.
    virtual float ReadFloat() = 0;

    // Reads a 64 bit double.
    virtual double ReadDouble() = 0;

    // Reads a boolean.
    virtual bool ReadBoolean() = 0;

    // Reads a String.
    virtual std::string ReadString() = 0;

    // Reads an array of bytes.
    virtual void const * Read(size_t len) = 0;
};

#endif // I_OBJECT_INPUT_STREAM_H
