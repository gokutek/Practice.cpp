#ifndef I_OBJECT_OUTPUT_STREAM_H
#define I_OBJECT_OUTPUT_STREAM_H

#include <string>
#include <stdint.h>

class IObjectOutputStream
{
public:
    virtual ~IObjectOutputStream() {}

    // Writes an 8 bit byte.
    virtual void WriteInt8(int8_t val) = 0;

    // Writes a 16 bit short.
    virtual void WriteInt16(int16_t val) = 0;

    // Writes a 32 bit int.
    virtual void WriteInt32(int32_t val) = 0;

    // Writes a 64 bit long.
    virtual void WriteInt64(int64_t val) = 0;

    // Writes a 32 bit float.
    virtual void WriteFloat(float val) = 0;

    // Writes a 64 bit double.
    virtual void WriteDouble(double val) = 0;

    // Writes a boolean.
    virtual void WriteBoolean(bool val) = 0;

    // Writes a String.
    virtual void WriteString(std::string const &str) = 0;

    // Writes an array of bytes.
    virtual void Write(void const *buf, size_t len) = 0;

    // Writes a sub array of bytes.
    virtual void Write(void const *buf, size_t off, size_t len) = 0;
};

#endif // I_OBJECT_OUTPUT_STREAM_H
