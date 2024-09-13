# tester
The tester tests C/C++ code.

### API
```c
// == Core ==
AddTest(Name)
AddSuite(Name)
RunTests()

// == Assertions ==
// Logical
AssertTrue(Actual)
AssertFalse(Actual)
// Equality
AssertEqual(Actual, Expected)
AssertNotEqual(Actual, Expected)
AssertEqualDelta(Actual, Expected, Epsilon)
AssertNotEqualDelta(Actual, Expected, Epsilon)
AssertDeepEqual(Actual, Expected, Comparator) // Comparator: true => 0
// Comparison
AssertLessThan(Value, Limit)
AssertLessThanOrEqual(Value, Limit)
AssertGreaterThan(Value, Limit)
AssertGreaterThanOrEqual(Value, Limit)
// Array/String
AssertArrayEqual(Actual, Expected, Length)
AssertArrayDeepEqual(Actual, Expected, Length, Comparator)  // Comparator: true => 0
AssertStringEqual(Actual, Expected)
```

### Example
```c
// test_one.h
#include "tester_assert.h"

// Step 1: Declare your tests
Test(TheFirstTest)
{
    AssertEqual(1, 2);
    return 0;
}

Test(TheSecondTest)
{
    AssertNotEqual(0, 1);
    return 0;
}

Test(TheThirdTest)
{
    int a[5] = { 1, 2, 3, 4, 5 };
    int b[5] = { 1, 2, 3, 6, 5 };
    AssertArrayEqual(a, b, 5);
    return 0;
}

struct the_thing {
    int foo;
    int bar;
};

int compare_the_thing(the_thing *a, the_thing *b) {
    return a->foo == b->foo && a->bar == b->bar ? 0 : -1; // Return 0 if equal, non-zero if not equal
}

Test(TheFourthTest)
{
    struct the_thing a = { .foo = 10, .bar = 20 };
    struct the_thing b = { .foo = 11, .bar = 20 };
    AssertDeepEqual(&a, &b, compare_the_thing);
}

// Step 2: Declare your test suite
TestSuite(TestOne)
{
    // Step 3: Add your tests to the test suite
    AddTest(TheFirstTest);
    AddTest(TheSecondTest);
    AddTest(TheThirdTest);
    AddTest(TheFourthTest);
}
```

```c
// main.c/cpp
#include "test_one.h"
#include "tester.h" // Note: Include tester.h below your test files

int main(void) {
    // Step 3: Add the test suite(s)
    AddSuite(TestOne);
    
    // Step 4: Run the tests
    RunTests();
}
```