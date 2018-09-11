#ifndef OBJECT_INPUT_STREAM_H
#define OBJECT_INPUT_STREAM_H

#include <assert.h>
#include <vector>
#include "i_object_input_stream.h"

class ObjectMemoryInputStream : public IObjectInputStream
{
public:
    explicit ObjectMemoryInputStream(std::vector<char> const &buffer);

    // Reads an 8 bit byte.
    int8_t ReadInt8() override;

    // Reads a 16 bit short.
    int16_t ReadInt16() override;

    // Reads a 32 bit int.
    int32_t ReadInt32() override;

    // Reads a 64 bit long.
    int64_t ReadInt64() override;

    // Reads a 32 bit float.
    float ReadFloat() override;

    // Reads a 64 bit double.
    double ReadDouble() override;

    // Reads a boolean.
    bool ReadBoolean() override;

    // Reads a String.
    std::string ReadString() override;

    // Reads an array of bytes.
    void const * Read(size_t len) override;

private:
    std::vector<char>   m_buffer;
    size_t              m_pos;
};


inline ObjectMemoryInputStream::ObjectMemoryInputStream(std::vector<char> const &buffer)
    : m_buffer(buffer),
    m_pos(0)
{
}

inline int8_t ObjectMemoryInputStream::ReadInt8()
{
    int8_t val;
    memcpy(&val, &m_buffer[m_pos], sizeof(val));
    m_pos += sizeof(val);
    return val;
}

inline int16_t ObjectMemoryInputStream::ReadInt16()
{
    int16_t val;
    memcpy(&val, &m_buffer[m_pos], sizeof(val));
    m_pos += sizeof(val);
    return val;
}

inline int32_t ObjectMemoryInputStream::ReadInt32()
{
    int32_t val;
    memcpy(&val, &m_buffer[m_pos], sizeof(val));
    m_pos += sizeof(val);
    return val;
}

inline int64_t ObjectMemoryInputStream::ReadInt64()
{
    int64_t val;
    memcpy(&val, &m_buffer[m_pos], sizeof(val));
    m_pos += sizeof(val);
    return val;
}

inline float ObjectMemoryInputStream::ReadFloat()
{
    float val;
    memcpy(&val, &m_buffer[m_pos], sizeof(val));
    m_pos += sizeof(val);
    return val;
}

inline double ObjectMemoryInputStream::ReadDouble()
{
    double val;
    memcpy(&val, &m_buffer[m_pos], sizeof(val));
    m_pos += sizeof(val);
    return val;
}

inline bool ObjectMemoryInputStream::ReadBoolean()
{
    bool val;
    memcpy(&val, &m_buffer[m_pos], sizeof(val));
    m_pos += sizeof(val);
    return val;
}

inline std::string ObjectMemoryInputStream::ReadString()
{
    int64_t const len = ReadInt64();
    if (len) 
    {
        char const *str = (char const*)Read((size_t)len);
        return std::string(str, str + len);
    }
    return "";
}

inline void const * ObjectMemoryInputStream::Read(size_t len)
{
    if (m_pos + len > m_buffer.size()) { return NULL; }
    void const *buf = &m_buffer[m_pos];
    m_pos += len;
    return buf;
}

#endif // OBJECT_INPUT_STREAM_H
