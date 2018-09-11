#ifndef OBJECT_OUTPUT_STREAM_H
#define OBJECT_OUTPUT_STREAM_H

#include <vector>
#include "i_object_output_stream.h"

class ObjectMemoryOutputStream : public IObjectOutputStream
{
public:
    // Writes an 8 bit byte.
    void WriteInt8(int8_t val) override;

    // Writes a 16 bit short.
    void WriteInt16(int16_t val) override;

    // Writes a 32 bit int.
    void WriteInt32(int32_t val) override;

    // Writes a 64 bit long.
    void WriteInt64(int64_t val) override;

    // Writes a 32 bit float.
    void WriteFloat(float val) override;

    // Writes a 64 bit double.
    void WriteDouble(double val) override;

    // Writes a boolean.
    void WriteBoolean(bool val) override;

    // Writes a String.
    void WriteString(std::string const &str) override;

    // Writes an array of bytes.
    void Write(void const *buf, size_t len) override;

    // Writes a sub array of bytes.
    void Write(void const *buf, size_t off, size_t len) override;

    std::vector<char> const & GetBuffer() const;

private:
    std::vector<char> m_buffer;
};

inline void ObjectMemoryOutputStream::WriteInt8(int8_t val)
{
    Write(&val, sizeof(val));
}

inline void ObjectMemoryOutputStream::WriteInt16(int16_t val)
{
    Write(&val, sizeof(val));
}

inline void ObjectMemoryOutputStream::WriteInt32(int32_t val)
{
    Write(&val, sizeof(val));
}

inline void ObjectMemoryOutputStream::WriteInt64(int64_t val)
{
    Write(&val, sizeof(val));
}

inline void ObjectMemoryOutputStream::WriteFloat(float val)
{
    Write(&val, sizeof(val));
}

inline void ObjectMemoryOutputStream::WriteDouble(double val)
{
    Write(&val, sizeof(val));
}

inline void ObjectMemoryOutputStream::WriteBoolean(bool val)
{
    Write(&val, sizeof(val));
}

inline void ObjectMemoryOutputStream::WriteString(std::string const &str)
{
    WriteInt64(str.size());
    Write(str.c_str(), str.size());
}

inline void ObjectMemoryOutputStream::Write(void const *buf, size_t len)
{
    Write(buf, 0, len);
}

inline void ObjectMemoryOutputStream::Write(void const *buf, size_t off, size_t len)
{
    size_t const pos = m_buffer.size();
    m_buffer.resize(m_buffer.size() + len);
    memcpy(&m_buffer[pos], (char const*)buf + off, len);
}

inline std::vector<char> const & ObjectMemoryOutputStream::GetBuffer() const
{
    return m_buffer;
}

#endif // OBJECT_OUTPUT_STREAM_H
