#include "arbitrage.hpp"
#include "test.hpp"

#include <functional>
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
    this->poolA->unsubscribe(this);
    this->poolB->unsubscribe(this);
    if (this->t.joinable())
    {
        this->t.join();
    }
}

void ArbitrageEngine::spawnThread()
{
    this->t = std::thread(&ArbitrageEngine::pollForUpdate, this);
}

void ArbitrageEngine::joinThread()
{
    if (this->t.joinable())
    {
        this->t.join();
    }
}

void ArbitrageEngine::onUpdate(Pool* p)
{
    this->onSwap(p);
}

void ArbitrageEngine::onSwap(Pool* p)
{
    updateQueue.push_back(p);
}

void ArbitrageEngine::pollForUpdate()
{
    while (true)
    {
        while(updateQueue.size() > 0)
        {
            Pool* p = updateQueue.front();
            // in an exchange we would have this information from the message -- break encapsulation as a mock up
            const Token& DAI = Test::Nutcracker::getX(*p);
            const Token& ETH = Test::Nutcracker::getY(*p);
            
            double relPrice = ETH.qty/DAI.qty;

            if (p == this->poolA)
            {
                this->lastRelativePriceA = relPrice;
            }
            else
            {
                this->lastRelativePriceB = relPrice;
            }
            
            if (this->lastRelativePriceA != this->lastRelativePriceB)
            {
                std::cout << "There is an arbitrage opportunity." << std::endl;
                //Todo -- workout arb values to calc ETH qty and profit
            }
            updateQueue.pop_front();
        }

        std::this_thread::sleep_for(std::chrono::nanoseconds(250));
    }
}
