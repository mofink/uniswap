#pragma once

#include <cassert>
#include <iostream>

#include "pool.hpp"

namespace Test
{
    // used to get at the internals of a Pool object for test
    struct Nutcracker
    {
        static const Token& getX(const Pool& p);
        static const Token& getY(const Pool& p);   
    };

void test1();
void test2();
void test3();

}