# uniswap

The core uniswap engine was written to be robust rather than performant. In contrast, the arbitrage logic was written to be performant

I did not have time to research the uniswap protocol extensively so I point out in code assumptions that I make which may diverge from the spec. In particular, it was not super clear to me how the invariant k comes into play (or whether it does at all) when adding liquidity to the pool. It was also unclear to me what the swap fee is a percentage of.

test1 -- basic add/remove liquidity operations
test2 -- swap operations
test3 -- swap operations including arbitrage routine