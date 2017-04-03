/*
 * Copyright (C) 2016-2017 Eistec AB
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "tests-cpp_ctors.h"
// #include <iostream>

volatile long tests_cpp_ctors_magic1 = 12345;
volatile long tests_cpp_ctors_magic2 = 11111111;
void *tests_cpp_ctors_order[8];

namespace RIOTTestCPP {

MyClass::MyClass()
{
//     std::cerr << "MyClass::MyClass()" << std::endl;
    var = tests_cpp_ctors_magic1;
}

MyClass::MyClass(long value) : var(value)
{
//     std::cerr << "MyClass::MyClass(" << value << ")" << std::endl;

}

long MyClass::value()
{
    return var;
}

void MyClass::inc()
{
    ++var;
}

} /* namespace RIOTTestCPP */
