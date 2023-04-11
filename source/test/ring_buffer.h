/*
===============================================================================
2023/04/10: 环形缓冲区
===============================================================================
*/

#pragma once

#include <assert.h>
#include <vector>
#include <stdint.h>

class ring_buffer
{
public:
	/// <summary>
	/// 创建一个缓冲区
	/// </summary>
	/// <param name="size">缓冲区字节数</param>
	ring_buffer(size_t size);

	/// <summary>
	/// 缓冲区中未读取的字节数
	/// </summary>
	/// <returns></returns>
	size_t get_unread_size() const;

	/// <summary>
	/// 读取缓冲区中的数据
	/// </summary>
	/// <param name="buffer">输出缓冲区</param>
	/// <param name="buffer_size">输出缓冲区字节数</param>
	/// <returns>实际读取的字节数</returns>
	size_t read(uint8_t* buffer, size_t buffer_size);

	/// <summary>
	/// 缓冲区的可写长度
	/// </summary>
	/// <returns></returns>
	size_t get_writable_size() const;

	/// <summary>
	/// 将数据写入缓冲区
	/// </summary>
	/// <param name="buffer">要写入的数据</param>
	/// <param name="buffer_size">要写入的数据长度</param>
	/// <returns>实际写入字节数</returns>
	size_t write(uint8_t* buffer, size_t buffer_size);

private:
	std::vector<uint8_t> buffer_;
	size_t read_pos_;
	size_t write_pos_;
};

inline ring_buffer::ring_buffer(size_t size)
{
	buffer_.resize(size + 1);
	read_pos_ = write_pos_ = 0;
}

inline size_t ring_buffer::get_unread_size() const
{
	if (read_pos_ == write_pos_)
	{
		return 0;
	}
	else if (read_pos_ < write_pos_)
	{
		return write_pos_ - read_pos_;
	}
	else
	{
		return buffer_.size() - read_pos_ + write_pos_;
	}
}

inline size_t ring_buffer::read(uint8_t* buffer, size_t buffer_size)
{
	size_t sz = std::min(buffer_size, get_unread_size());
	for (size_t i = 0; i < sz; ++i)
	{
		buffer[i] = buffer_[read_pos_++];

		if (read_pos_ >= buffer_.size())
		{
			read_pos_ = 0;
		}
	}
	return sz;
}

inline size_t ring_buffer::get_writable_size() const
{
	if (read_pos_ == write_pos_)
	{
		return buffer_.size() - 1;
	}
	else if (read_pos_ < write_pos_)
	{
		return buffer_.size() - (write_pos_ - read_pos_);
	}
	else
	{
		return read_pos_ - write_pos_;
	}
}

inline size_t ring_buffer::write(uint8_t* buffer, size_t buffer_size)
{
	size_t sz = std::min(buffer_size, get_writable_size());
	for (size_t i = 0; i < sz; ++i)
	{
		buffer_[write_pos_++] = buffer[i];

		if (write_pos_ >= buffer_.size())
		{
			write_pos_ = 0;
		}
	}
	return sz;
}

/*
===============================================================================
空
满
===============================================================================
*/
