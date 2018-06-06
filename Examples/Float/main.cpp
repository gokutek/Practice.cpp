#include <iostream>
#include <math.h>
#include <numeric>
#include <limits>


int main()
{
    float const maxFloat = std::numeric_limits<float>::max();
    float const minFloat = std::numeric_limits<float>::min();
    std::cout << (int)maxFloat << std::endl;
    std::cout << (int)minFloat << std::endl;
    float x = -226423088.0f;
    x += 1;
    bool b = isnan(x);
    return 0;
}
