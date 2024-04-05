#pragma once

//Pool class

#include <string>
#include <cstdint>

namespace Test
{
    struct Nutcracker;
};

struct Token
{
    std::string name;
    //fractional shares from added fees
    double qty;
};

//assumes CFMM
struct Pool
{
    // breaks encapsulation to get at the internals of a Pool object
    friend class Test::Nutcracker;

    Pool(const std::string& poolName, const std::string& tokenX, const std::string& tokenY, const double swapFee = 0.03);
    ~Pool() {};

    Pool(const Pool&) = delete;
    Pool& operator=(const Pool&) = delete;

    // implicitly adds/removes the other token qty (I don't know what the real API looks like)
    void addLiquidity(const std::string& token, uint64_t qty);
    void removeLiquidity(const std::string& token, uint64_t qty);

    void swapTokens(const std::string& inputToken, uint64_t inQty);

private:
    std::string poolName;

    Token X;
    Token Y;

    double swapFee;
    
    //token equivalence when adding liquidity
    double tokenEq;
};