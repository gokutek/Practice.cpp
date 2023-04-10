/*
 * 2023/04/10: ≤‚ ‘ring_buffer
 */

#include "ring_buffer.h"
#include <assert.h>

int main()
{
	ring_buffer rbuffer(16);
	size_t sz = rbuffer.get_unread_size();
	sz = rbuffer.get_writable_size();
	char buffer[] = "123451234512345123451234512345";
	sz = rbuffer.write((uint8_t*)buffer, 16);
	char tmp[32] = { 0 };
	sz = rbuffer.read((uint8_t*)tmp, 32);
	return 0;
}
