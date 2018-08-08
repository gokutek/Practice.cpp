#include <boost/preprocessor.hpp>

int main()
{
    int xx = BOOST_PP_VARIADIC_SIZE();
    int x = BOOST_PP_VARIADIC_SIZE(1, 2, 3);
    return 0;
}
