/*
 * 2023/04/10: ≤‚ ‘ring_buffer
 */

#include "ring_buffer.h"
#include <assert.h>

int main()
{
	int const kBufferSize = 8;
	ring_buffer rbuffer(kBufferSize);
	
	assert(0 == rbuffer.get_unread_size());
	assert(rbuffer.get_writable_size() == 8);

	{
		char const buffer[] = "12345678";
		assert(rbuffer.write((uint8_t*)buffer, 8) == 8);
		assert(rbuffer.get_unread_size() == 8);
		char tmp[32] = { 0 };
		assert(rbuffer.read((uint8_t*)tmp, 32) == 8);
		assert(!strcmp(tmp, buffer));
		assert(0 == rbuffer.get_unread_size());
		assert(rbuffer.get_writable_size() == 8);
	}

	{
		char const buffer[] = "12345";
		assert(rbuffer.write((uint8_t*)buffer, 5) == 5);
		assert(rbuffer.get_unread_size() == 5);
		char tmp[32] = { 0 };
		assert(rbuffer.read((uint8_t*)tmp, 32) == 5);
		assert(!strcmp(tmp, buffer));
		assert(0 == rbuffer.get_unread_size());
		assert(rbuffer.get_writable_size() == 8);
	}

	{
		char const buffer[] = "12345678";
		assert(rbuffer.write((uint8_t*)buffer, 8) == 8);
		assert(rbuffer.get_unread_size() == 8);
		char tmp[32] = { 0 };
		assert(rbuffer.read((uint8_t*)tmp, 32) == 8);
		assert(!strcmp(tmp, buffer));
		assert(0 == rbuffer.get_unread_size());
		assert(rbuffer.get_writable_size() == 8);
	}

	return 0;
}
