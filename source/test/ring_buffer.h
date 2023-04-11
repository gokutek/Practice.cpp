/*
===============================================================================
2023/04/10: ���λ�����
===============================================================================
*/

#pragma once

#include <assert.h>
#include <vector>
#include <stdint.h>
#include <tuple>

class ring_buffer
{
public:
	/// <summary>
	/// ����һ��������
	/// </summary>
	/// <param name="size">�������ֽ���</param>
	ring_buffer(size_t size);

	/// <summary>
	/// ��������δ��ȡ���ֽ���
	/// </summary>
	/// <returns></returns>
	size_t get_unread_size() const;

	/// <summary>
	/// ��ȡ�������е�����
	/// </summary>
	/// <param name="buffer">���������</param>
	/// <param name="buffer_size">����������ֽ���</param>
	/// <returns>ʵ�ʶ�ȡ���ֽ���</returns>
	size_t read(uint8_t* buffer, size_t buffer_size);

	/// <summary>
	/// �������Ŀ�д����
	/// </summary>
	/// <returns></returns>
	size_t get_writable_size() const;

	/// <summary>
	/// ������д�뻺����
	/// </summary>
	/// <param name="buffer">Ҫд�������</param>
	/// <param name="buffer_size">Ҫд������ݳ���</param>
	/// <returns>ʵ��д���ֽ���</returns>
	size_t write(uint8_t* buffer, size_t buffer_size);

	/// <summary>
	/// ��ȡ��д�����飬��������β2��
	/// </summary>
	/// <returns></returns>
	std::vector<std::tuple<uint8_t*,size_t> > get_writable_sections();

	/// <summary>
	/// ��ǰ�ƶ�дָ��λ��
	/// </summary>
	/// <param name="offset"></param>
	void advance_write_pos(size_t offset);

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
	size_t sz = (std::min)(buffer_size, get_unread_size());
#if 0
	for (size_t i = 0; i < sz; ++i)
	{
		buffer[i] = buffer_[read_pos_++];

		if (read_pos_ >= buffer_.size())
		{
			read_pos_ = 0;
		}
	}
#else
	if (read_pos_ + sz >= buffer_.size())
	{
		size_t sz1 = buffer_.size() - read_pos_;
		memcpy(buffer, &buffer_[read_pos_], sz1);

		size_t sz2 = sz - sz1;
		memcpy(buffer + sz1, &buffer_[0], sz2);

		read_pos_ = sz2;
	}
	else
	{
		memcpy(buffer, &buffer_[read_pos_], sz);
		read_pos_ += sz;
	}
#endif
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
	size_t sz = (std::min)(buffer_size, get_writable_size());
#if 0
	for (size_t i = 0; i < sz; ++i)
	{
		buffer_[write_pos_++] = buffer[i];

		if (write_pos_ >= buffer_.size())
		{
			write_pos_ = 0;
		}
	}
#else
	if (write_pos_ + sz >= buffer_.size())
	{
		size_t sz1 = buffer_.size() - write_pos_;
		memcpy(&buffer_[write_pos_], buffer, sz1);

		size_t sz2 = sz - sz1;
		memcpy(&buffer_[0], buffer + sz1, sz2);

		write_pos_ = sz2;
	}
	else
	{
		memcpy(&buffer_[write_pos_], buffer, sz);
		write_pos_ += sz;
	}
#endif
	return sz;
}

inline std::vector<std::tuple<uint8_t*, size_t> > ring_buffer::get_writable_sections()
{
	std::vector<std::tuple<uint8_t*, size_t> > sections;

	size_t sz = get_writable_size();

	if (sz > 0)
	{
		if (write_pos_ + sz >= buffer_.size())
		{
			size_t sz1 = buffer_.size() - write_pos_;
			sections.push_back(std::make_tuple(&buffer_[write_pos_], sz1));

			size_t sz2 = sz - sz1;
			sections.push_back(std::make_tuple(&buffer_[0], sz2));
		}
		else
		{
			sections.push_back(std::make_tuple(&buffer_[write_pos_], sz));
		}
	}

	return sections;
}

inline void ring_buffer::advance_write_pos(size_t offset)
{
	size_t sz = (std::min)(offset, get_writable_size());

	if (write_pos_ + sz >= buffer_.size())
	{
		size_t sz1 = buffer_.size() - write_pos_;
		size_t sz2 = sz - sz1;
		write_pos_ = sz2;
	}
	else
	{
		write_pos_ += sz;
	}
}

/*
===============================================================================
��
��
===============================================================================
*/
