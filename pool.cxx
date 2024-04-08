#include "pool.hpp"

#include <cmath>
#include <iostream>

Pool::Pool(const std::string& poolName, const std::string& tokenX, const std::string& tokenY, const double swapFee)
: poolName(poolName)
, tokenEq(tokenEq)
, swapFee(swapFee)
{
    //assume 3:1 initial value

    X.name = tokenX;
    X.qty = 1200.0;

    Y.name = tokenY;
    Y.qty = 400.0;

    tokenEq = X.qty/Y.qty;
}

void Pool::addLiquidity(const std::string& token, uint64_t qty)
{
    double otherQty;
    std::string otherToken;

    if (token == this->X.name)
    {
        otherToken = this->Y.name;
        otherQty = (1/this->tokenEq)*qty;
        
        X.qty += qty;
        Y.qty += otherQty;
    }
    else if (token == this->Y.name)
    {
        otherToken = this->X.name;
        otherQty = this->tokenEq*qty;

        Y.qty += qty;
        X.qty += otherQty;
    }
    else
    {
        std::cout << "[LOG_WARN] Invalid token name " << token << "; could not add liquidity to Pool" << this->poolName << std::endl;
        return;
    }

    std::cout << "[LOG_INFO] Added liquidity to Pool: name=" << this->poolName << " token=" << token << " qty=" << qty 
        << "; token=" << otherToken << " qty=" << otherQty <<  std::endl;
}

void Pool::removeLiquidity(const std::string& token, uint64_t qty)
{
    //ToDo -- make sure there is enough liquidity available to remove

    double otherQty;
    std::string otherToken;

    if (token == this->X.name)
    {
        otherToken = this->Y.name;
        otherQty = (1/this->tokenEq)*qty;

        X.qty -= qty;
        Y.qty -= otherQty;
    }
    else if (token == this->Y.name)
    {
        otherToken = this->X.name;
        otherQty = this->tokenEq*qty;

        Y.qty -= qty;
        X.qty -= otherQty;
    }
    else
    {
        std::cout << "[LOG_WARN] Invalid token name " << token << "; could not remove liquidity from Pool" << this->poolName << std::endl;
        return;
    }

    std::cout << "[LOG_INFO] Removed liquidity from Pool: name=" << this->poolName << " token=" << token << " qty=" << qty 
        << "; token=" << otherToken << " qty=" << otherQty <<  std::endl;
}

void Pool::swapTokens(const std::string &inputToken, uint64_t inQty)
{
    Token* inToken;
    Token* outToken;

    double totalOutQty = 0;

    if (inputToken == this->X.name)
    {
        inToken = &this->X;
        outToken = &this->Y;
    }
    else if (inputToken == this->Y.name)
    {
        inToken = &this->Y;
        outToken = &this->X;
    }
    else
    {
        std::cout << "[LOG_WARN] Invalid input token name " << inputToken << "; could not execute swap in Pool" << this->poolName << std::endl;
        return;
    }

    uint64_t needsFill = inQty;
    while (needsFill)
    {
        // according to the spec, prices iteratively get worse
        double outQty = outToken->qty / inToken->qty;
        totalOutQty += outQty;

        if (totalOutQty > outToken->qty)
        {
            //restore the removed qty and abort. this assumes that if we can't fill the full order then we don't fill any of it (need to confirm in the spec)
            std::cout << "[LOG_WARN] Insufficient liquidity reserves; could not execute swap in Pool" << this->poolName << std::endl;
            return;
        }

        needsFill--;
    }
    
    inToken->qty += static_cast<double>(inQty);
    outToken->qty -= totalOutQty;

    std::cout << "[LOG_INFO] Swapping tokens in Pool: name=" << this->poolName << " inputToken=" << inToken->name << " outputToken=" << outToken->name 
       << " inQty=" << inQty << " outQty=" << totalOutQty << std::endl;

    inToken->qty += this->swapFee;

    std::cout << "[LOG_INFO] After fees the reserves in Pool " << this->poolName << " are: token=" << this->X.name << " qty=" << this->X.qty 
        << "; token=" << this->Y.name << " qty=" <<this->Y.qty << std::endl; 

    // invoke callbacks to all subscribers
    notify();
}
