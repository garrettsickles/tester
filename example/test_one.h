#include "tester_assert.h"

Test(TheFirstTest)
{
    AssertEqualDelta(100, 102, 2);
    AssertNotEqualDelta(100, 103, 2);
    AssertEqual(1, 2);
    return 0;
}

Test(TheSecondTest)
{
    AssertEqual(2, 2);
    AssertTrue(1);
    AssertFalse(11);
    AssertNotEqual(0, 1);
    return 0;
}

Test(TheThirdTest)
{
    int a[5] = { 1, 2, 3, 4, 5 };
    int b[5] = { 1, 2, 3, 6, 5 };

    AssertArrayEqual(a, b, 3);
    AssertArrayEqual(a, b, 5);

    return 0;
}

int custom_compare(int a, int b) {
    return b - a <= 0;
}

Test(TheFourthTest)
{
    int a[5] = { 1, 2, 3, 4, 5 };
    int b[5] = { 1, 2, 3, 6, 5 };

    AssertArrayDeepEqual(a, b, 5, custom_compare);

    return 0;
}

struct the_thing {
    int foo;
    int bar;
};

int compare_the_thing(struct the_thing *a, struct the_thing *b) {
    return a->foo == b->foo && a->bar == b->bar ? 0 : -1; // Return 0 if equal, non-zero if not equal
}

Test(TheFifthTest)
{
    struct the_thing a = { .foo = 10, .bar = 20 };
    struct the_thing b = { .foo = 11, .bar = 20 };
    AssertDeepEqual(&a, &b, compare_the_thing);
}

TestSuite(TestOne)
{
    AddTest(TheFirstTest);
    AddTest(TheSecondTest);
    AddTest(TheThirdTest);
    AddTest(TheFourthTest);
    AddTest(TheFifthTest);
}