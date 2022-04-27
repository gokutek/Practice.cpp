#include <windows.h>

// �ַ���ת�������ַ���
static const wchar_t* PlatformStringToWideStr(const char* info, wchar_t* buf, size_t byte_size)
{
	const size_t len = byte_size / sizeof(wchar_t);
	int res = MultiByteToWideChar(CP_ACP, 0, info, -1, buf, int(len));

	if (res == 0)
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			buf[len - 1] = 0;
		}
		else
		{
			buf[0] = 0;
		}
	}

	return buf;
}

// ���ַ���ת�����ַ���
static const char* PlatformWideStrToString(const wchar_t* info, char* buf, size_t byte_size)
{
	int res = WideCharToMultiByte(CP_ACP, 0, info, -1, buf, int(byte_size),
		nullptr, nullptr);

	if (0 == res)
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			buf[byte_size - 1] = 0;
		}
		else
		{
			buf[0] = 0;
		}
	}

	return buf;
}

// ���ַ���ת����UTF8
static const char* PlatformWideStrToUTF8(const wchar_t* info, char* buf, size_t byte_size)
{
	size_t src_len = wcslen(info);

	if (0 == src_len)
	{
		buf[0] = 0;
		return buf;
	}

	size_t max_len = byte_size - 1;
	size_t count = 0;

	for (size_t i = 0; i < src_len; ++i)
	{
		wchar_t wch = info[i];

		if (wch < 0x80)	// 0xxx xxxx
		{
			if ((count + 1) > max_len)
			{
				break;
			}

			buf[count++] = (char)wch;
		}
		else if (wch < 0x800) // 110x xxxx 10xx xxxx
		{
			if ((count + 2) > max_len)
			{
				break;
			}

			buf[count++] = 0xC0 | (wch >> 6);
			buf[count++] = 0x80 | (wch & 0x3F);
		}
		else if (wch < 0x10000)	// 1110 xxxx 10xx xxxx 10xx xxxx
		{
			if ((count + 3) > max_len)
			{
				break;
			}

			buf[count++] = 0xE0 | (wch >> 12);
			buf[count++] = 0x80 | ((wch >> 6) & 0x3F);
			buf[count++] = 0x80 | (wch & 0x3F);
		}
		else if (wch < 0x110000) // UCS4: 1111 0xxx 10xx xxxx 10xx xxxx 10xx xxxx
		{
			if ((count + 4) > max_len)
			{
				break;
			}

			buf[count++] = 0xF0 | (wch >> 18);
			buf[count++] = 0x80 | ((wch >> 12) & 0x3F);
			buf[count++] = 0x80 | ((wch >> 6) & 0x3F);
			buf[count++] = 0x80 | (wch & 0x3F);
		}
		else // UCS4: 1111 10xx 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx
		{
			if ((count + 5) > max_len)
			{
				break;
			}

			buf[count++] = 0xF8 | (wch >> 24);
			buf[count++] = 0x80 | ((wch >> 18) & 0x3F);
			buf[count++] = 0x80 | ((wch >> 12) & 0x3F);
			buf[count++] = 0x80 | ((wch >> 6) & 0x3F);
			buf[count++] = 0x80 | (wch & 0x3F);
		}
	}

	buf[count] = 0;

	return buf;
}

int main()
{	
	//Lua��XML����UTF-8
	char szUTF8[256] = { 0 };
	PlatformWideStrToUTF8(L"�й�", szUTF8, sizeof(szUTF8));

	//ת�ɿ��ַ����ڲ���Ϊ��ANSI���룩
	wchar_t wszBuff[256] = { 0 };
	PlatformStringToWideStr(szUTF8, wszBuff, sizeof(wszBuff));

	/////////////////////////////////////////////////////////
	//�����ǿͻ����ٳ���ת��UTF-8
	char szTestUTF8[256] = { 0 };
	PlatformWideStrToUTF8(wszBuff, szTestUTF8, sizeof(szTestUTF8));

	//szUTF8��szTestUTF8����~

	return 0;
}
