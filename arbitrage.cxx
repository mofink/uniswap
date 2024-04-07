#include "arbitrage.hpp"

#include <iostream>

ArbitrageEngine::ArbitrageEngine(Pool* poolA, Pool* poolB)
: poolA(poolA)
, poolB(poolB)
{
    poolA->subscribe(this);
    poolB->subscribe(this);
}

ArbitrageEngine::~ArbitrageEngine()
{
    poolA->unsubscribe(this);
    poolB->unsubscribe(this);
}

void ArbitrageEngine::onUpdate(Pool *p)
{
    onSwap(p);
}

void ArbitrageEngine::onSwap(Pool *)
{
    std::cout << "Alerted that a swap took place" << std::endl;
}
