#include "observer.hpp"
#include "pool.hpp"

//simulates a client subscribed to exchange updates
struct ArbitrageEngine : public IObserver<Pool>
{
    // it's a little wonky to have the pools passed as an arg to the arbitrage engine's ctor; in a real system the exchange would provide some kind 
    // of API to register listeners
    ArbitrageEngine(Pool*, Pool*);
    ~ArbitrageEngine();

    // registered to be called whenever a swap is executed
    void onUpdate(Pool*);

private:
    void onSwap(Pool*);

    Pool* poolA;
    Pool* poolB;
};