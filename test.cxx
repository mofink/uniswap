#include "test.hpp"

#include <math.h>
#include <limits>

#define ASSERT(condition) { if(!(condition)){ std::cout << "ASSERT FAILED: " << #condition << " @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; } }
#define DOUBLE_EQ(A,B) std::fabs(A - B) <= std::numeric_limits<double>::epsilon()

namespace Test
{

const Token& Nutcracker::getX(const Pool& p)
{
    return p.X;
}

const Token& Nutcracker::getY(const Pool& p)
{
    return p.Y;
}

void test1()
{
    Pool poolA("A", "DAI", "ETH");
    const Token& DAI = Nutcracker::getX(poolA);
    const Token& ETH = Nutcracker::getY(poolA);
    ASSERT(ETH.qty == 400.0);
    ASSERT(DAI.qty == 1200.0);

    poolA.addLiquidity("ETH", 1.0);
    ASSERT(ETH.qty == 401.0);
    ASSERT(DAI.qty == 1203.0);

    poolA.addLiquidity("ETH", 1.0);
    ASSERT(ETH.qty == 402.0);
    ASSERT(DAI.qty == 1206.0);

    poolA.addLiquidity("DAI", 6.0);
    ASSERT(ETH.qty == 404.0);
    ASSERT(DAI.qty == 1212.0);

    poolA.removeLiquidity("ETH", 1.0);
    ASSERT(ETH.qty == 403.0);
    ASSERT(DAI.qty == 1209.0);
}

void test2()
{
    Pool poolA("A", "DAI", "ETH");
    const Token& DAI = Nutcracker::getX(poolA);
    const Token& ETH = Nutcracker::getY(poolA);
    ASSERT(ETH.qty == 400.0);
    ASSERT(DAI.qty == 1200.0);

    poolA.swapTokens("DAI", 3.0);
    ASSERT(ETH.qty == 399.0);
    ASSERT(!DOUBLE_EQ(DAI.qty, 1203.3));
}

}