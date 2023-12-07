#include <string.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>

int main()
{
	char buffer[256];

	sprintf_s(buffer, "%d", 100);
	assert(!strcmp(buffer, "100"));

#pragma warning(push)
#pragma warning(disable: 4477)
	//类型不匹配，并不会先将float转成int再输出
	sprintf_s(buffer, "%d", 100.0f);
	assert(!strcmp(buffer, "0"));

	//同理，这里也不会将int转成float再输出
	sprintf_s(buffer, "%f", 100);
	assert(strcmp(buffer, "100"));
#pragma warning(pop)
	
	return 0;
}
