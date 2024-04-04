#include "pool.hpp"

#include <iostream>

Pool::Pool(const std::string& poolName, const std::string& tokenX, const std::string& tokenY, const double swapFee)
: poolName(poolName)
, swapFee(swapFee)
{
    X.name = tokenX;
    X.qty = 0.0;

    Y.name = tokenY;
    Y.qty = 0.0;
}

void Pool::addLiquidity(const std::string& token, uint64_t qty)
{
    // just assume that the tokens initially have 3:1 value
    const double invariant = (this->X.qty == 0 || this->Y.qty == 0) ? 3 : this->X.qty*this->Y.qty;
    double otherQty;
    std::string otherToken;

    if (token == this->X.name)
    {
        otherToken = this->Y.name;
        
        X.qty += qty;
        otherQty = invariant/X.qty;
        Y.qty += otherQty;
    }
    else if (token == this->Y.name)
    {
        otherToken = this->X.name;

        Y.qty += qty;
        otherQty = invariant/Y.qty;
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

    const double invariant = (this->X.qty == 0 || this->Y.qty == 0) ? 3 : this->X.qty*this->Y.qty;
    double otherQty;
    std::string otherToken;

    if (token == this->X.name)
    {
        otherToken = this->Y.name;
        
        X.qty -= qty;
        otherQty = invariant/X.qty;
        Y.qty -= otherQty;
    }
    else if (token == this->Y.name)
    {
        otherToken = this->Y.name;

        Y.qty -= qty;
        otherQty = invariant/Y.qty;
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

    uint64_t totalOutQty = 0;

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
        // according to the spec, prices iteratively get worse. I assume we always round down so that tokens are traded in integer amounts.
        uint64_t outQty = static_cast<uint64_t>(inToken->qty / outToken->qty);
        totalOutQty += outQty;
        inToken->qty++;
        outToken->qty -= outQty;

        if (outToken->qty < 0)
        {
            //restore the removed qty and abort. this assumes that if we can't fill the full order then we don't fill any of it (need to confirm in the spec)
            outToken->qty += totalOutQty;
            std::cout << "[LOG_WARN] Insufficient liquidity reserves; could not execute swap in Pool" << this->poolName << std::endl;
            return;
        }

        needsFill--;
    }

    std::cout << "[LOG_INFO] Swapping tokens in Pool: name=" << this->poolName << "inputToken=" << inToken->name << "outputToken=" << outToken->name 
        << " inQty=" << inQty << " outQty=" << totalOutQty << std::endl;

    inToken->qty += this->swapFee*static_cast<double>(inQty);

    std::cout << "[LOG_INFO] After fees the reserves in Pool " << this->poolName << " are: token= " << this->X.name << " qty=" 
        << "; token=" << this->Y.name << " qty=" <<this->Y.qty; 

}
