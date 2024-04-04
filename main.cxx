#include <iostream>

#include "pool.hpp"

void test1()
{
    Pool poolA("A", "DAI", "ETH");
    //Pool poolB = Pool("B", "DAI", "ETH");

    poolA.addLiquidity("ETH", 1);
    poolA.addLiquidity("ETH", 1);
    poolA.addLiquidity("ETH", 100);
    poolA.removeLiquidity("ETH", 102);
}

int main()
{
    test1();    
}