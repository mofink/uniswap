#pragma once

#include <cassert>
#include <iostream>

#include "pool.hpp"

namespace Test
{
    // used to get at the internals of a Pool object
    //should go into its own file but it is late and I don't want to reorg now
    struct Nutcracker
    {
        static const Token& getX(const Pool& p);
        static const Token& getY(const Pool& p);   
    };

void test1();
void test2();
void test3();

}