# uniswap

This project was developed and tested using gcc13/Centos 7

To build, invoke ./build.sh

To run, invoke ./run.sh

The core uniswap engine was written to be robust rather than performant

I did not have time to research the uniswap protocol extensively so I point out in code assumptions that I make which may diverge from the spec. In particular, it was not super clear to me how the invariant k comes into play when adding liquidity to the pool (if it does at all). It was also unclear to me what the swap fee is a percentage of.

test1 -- basic add/remove liquidity operations

test2 -- swap operations

test3 -- swap operations including arbitrage routine (runs forever since "market updates" will continue to come in)

To simulate a trading engine hooked into an exchange, I implemented the subject/observer pattern to register callbacks -- the exchange notifies listeners (here, the arbitrage engine) of market updates. In an effort to be somewhat extensible I created generic base classes (virtual interfaces) for Subjects and Observers, avoiding the costs of dynamic polymorphism using the CRTP https://eli.thegreenplace.net/2011/05/17/the-curiously-recurring-template-pattern-in-c/ Next steps would include message types, since a real exchange would presumably want to send updates besides for swap notifications.

I intentionally break encapsulation in the context of testing and receiving messages to simplify the proof-of-concept implementaion (noted in code).

The thread-safe queue was taken from https://stackoverflow.com/a/36763257
