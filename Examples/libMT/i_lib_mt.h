#ifndef I_LIB_MT_H
#define I_LIB_MT_H

#include <string>

class ITest
{
public:
	virtual ~ITest() = default;

	virtual std::string Crash() = 0;
};

#endif // I_LIB_MT_H
