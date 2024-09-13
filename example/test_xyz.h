#include "tester_assert.h"

#include <stdio.h>

Test(TheABCTest)
{
    AssertLessThan(1, 2);
    AssertLessThanOrEqual(3, 2);
    return 0;
}

Test(TheDEFTest)
{
    AssertGreaterThanOrEqual(2, 2);
    AssertGreaterThan(3, 4);
    return 0;
}

Test(TheGHITest)
{
    AssertGreaterThanOrEqual(2, 2);
    AssertGreaterThan(5, 4);
    return 0;
}

Test(TheJKLTest)
{
    const char* a = "This is a test string";
    const char* b = "This is a test string";
    const char* c = "This is a tezt string";
    AssertStringEqual(a, b);
    AssertStringEqual(a, c);
    return 0;
}

TestSuite(TestXYZ)
{
    AddTest(TheABCTest);
    AddTest(TheDEFTest);
    AddTest(TheGHITest);
    AddTest(TheJKLTest);
}