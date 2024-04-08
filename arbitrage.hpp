#include "observer.hpp"
#include "pool.hpp"
#include "shared_queue.hpp"

#include <thread>

//simulates a client subscribed to exchange updates
struct ArbitrageEngine : public IObserver<Pool>
{
    ArbitrageEngine(Pool*, Pool*);
    ~ArbitrageEngine();

    void spawnThread();
    //used to manually exit the process without signaling an interupt/kill
    void joinThread();

    void pollForUpdate();

private:

    // registered to be called whenever a swap is executed
    void onUpdate(Pool*);

    void onSwap(Pool*);

    Pool* poolA;
    Pool* poolB;

    // cache the results of the last calculation so that on notification we only need to update one pool's relative price 
    double lastRelativePriceA;
    double lastRelativePriceB;

    SharedQueue<Pool*> updateQueue;
    std::thread t;
};