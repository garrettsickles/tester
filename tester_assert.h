#ifndef TESTER_TESTER_ASSERT_H
#define TESTER_TESTER_ASSERT_H

// MIT License
//
// Copyright (c) 2024 Garrett Sickles
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#define CTESTER_STRINGIFY(X) CTESTER_STRINGIFY_(X)
#define CTESTER_STRINGIFY_(X) #X
#define CTESTER_STRINGCOPY(CTESTER_DST,CTESTER_STR) \
do {\
    long CTESTER_STRINGCOPY_IDX = -1;\
    while (CTESTER_STR[++CTESTER_STRINGCOPY_IDX] != 0)\
        CTESTER_DST[CTESTER_STRINGCOPY_IDX] = CTESTER_STR[CTESTER_STRINGCOPY_IDX];\
    CTESTER_DST[CTESTER_STRINGCOPY_IDX] = 0;\
} while (0)

#ifndef CTESTER_TYPES_DECL
#define CTESTER_TYPES_DECL
typedef void (*CTESTER_HELPER_TYPE)(void);
typedef int (*CTESTER_FUNCTION_TYPE)(void);
struct CTesterExecutor
{
    const CTESTER_HELPER_TYPE before;
    const CTESTER_HELPER_TYPE after;
    const CTESTER_FUNCTION_TYPE test;
    struct CTesterExecutor* prev;
    struct CTesterExecutor* next;
};
#endif

#define Test(CTESTER_INSTANCE_NAME)\
extern char CTESTER_CURRENT_SUITE_NAME[512];\
extern char CTESTER_CURRENT_INSTANCE_NAME[512];\
extern long CTESTER_SUCCESS_ASSERTION_COUNTER;\
extern long CTESTER_CURRENT_INSTANCE_INDEX;\
extern long CTESTER_TOTAL_INSTANCE_COUNT;\
extern long CTESTER_ARRAY_TEST;\
extern long CTESTER_ARRAY_TEST_INDEX;\
extern long CTESTER_FAILURE_CODE;\
extern char CTESTER_FAILURE_ACTUAL[512];\
extern char CTESTER_FAILURE_OTHER[512];\
extern char CTESTER_FAILURE_MESSAGE[512];\
extern char CTESTER_FAILURE_FILE[2048];\
extern char CTESTER_FAILURE_LINE[16];\
extern struct CTesterExecutor* CTESTER_CURRENT_TEST_INSTANCE;\
void CTESTER_INSTANCE_NAME ## _BeforeTest(void);\
void CTESTER_INSTANCE_NAME ## _AfterTest(void);\
int CTESTER_INSTANCE_NAME(void);\
struct CTesterExecutor CTESTER_INSTANCE_NAME ## _InstanceExecutor = { .before = CTESTER_INSTANCE_NAME ## _BeforeTest, .after = CTESTER_INSTANCE_NAME ## _AfterTest, .test = CTESTER_INSTANCE_NAME, .prev = 0, .next = 0 };\
void CTESTER_INSTANCE_NAME ## _PrepareTest(void)\
{\
    CTESTER_CURRENT_TEST_INSTANCE->next = &(CTESTER_INSTANCE_NAME ## _InstanceExecutor);\
    (&(CTESTER_INSTANCE_NAME ## _InstanceExecutor))->prev = CTESTER_CURRENT_TEST_INSTANCE;\
    CTESTER_CURRENT_TEST_INSTANCE = CTESTER_CURRENT_TEST_INSTANCE->next;\
}\
void CTESTER_INSTANCE_NAME ## _BeforeTest(void)\
{\
    CTESTER_STRINGCOPY(CTESTER_CURRENT_INSTANCE_NAME, CTESTER_STRINGIFY(CTESTER_INSTANCE_NAME));\
    CTESTER_SUCCESS_ASSERTION_COUNTER = 0;\
}\
void CTESTER_INSTANCE_NAME ## _AfterTest(void)\
{\
    return;\
}\
int CTESTER_INSTANCE_NAME(void)

#define TestSuite(CTESTER_SUITE_NAME)\
extern char CTESTER_CURRENT_SUITE_NAME[512];\
extern char CTESTER_CURRENT_INSTANCE_NAME[512];\
void CTESTER_SUITE_NAME ## _BeforeSuite(void);\
struct CTesterExecutor CTESTER_SUITE_NAME ## _SuiteExecutor = { .before = CTESTER_SUITE_NAME ## _BeforeSuite, .after = 0, .test = 0, .prev = 0, .next = 0 };\
void CTESTER_SUITE_NAME(void);\
void CTESTER_SUITE_NAME ## _PrepareSuite(void)\
{\
    CTESTER_CURRENT_TEST_INSTANCE->next = &(CTESTER_SUITE_NAME ## _SuiteExecutor);\
    (&(CTESTER_SUITE_NAME ## _SuiteExecutor))->prev = CTESTER_CURRENT_TEST_INSTANCE;\
    CTESTER_CURRENT_TEST_INSTANCE = CTESTER_CURRENT_TEST_INSTANCE->next;\
    CTESTER_SUITE_NAME();\
}\
void CTESTER_SUITE_NAME ## _BeforeSuite(void)\
{\
    CTESTER_STRINGCOPY(CTESTER_CURRENT_SUITE_NAME, CTESTER_STRINGIFY(CTESTER_SUITE_NAME));\
    return;\
}\
void CTESTER_SUITE_NAME(void)

#define AddTest(CTESTER_INSTANCE_NAME) CTESTER_INSTANCE_NAME ## _PrepareTest()
#define AddSuite(CTESTER_SUITE_NAME) CTESTER_SUITE_NAME ## _PrepareSuite()

#define CTesterAssertionNoOp() do {} while (0)
#define CTesterAssertionStateReset() do { \
    CTESTER_ARRAY_TEST = 0;\
    CTESTER_ARRAY_TEST_INDEX = -1;\
} while (0)
#define CTesterAssertionSuccess() CTESTER_SUCCESS_ASSERTION_COUNTER += 1
#define CTesterAssertionFailure(ACTUAL,MESSAGE,CODE)\
do {\
    CTESTER_STRINGCOPY(CTESTER_FAILURE_ACTUAL, CTESTER_STRINGIFY(ACTUAL));\
    CTESTER_STRINGCOPY(CTESTER_FAILURE_MESSAGE, CTESTER_STRINGIFY(MESSAGE));\
    CTESTER_STRINGCOPY(CTESTER_FAILURE_FILE, __FILE__);\
    CTESTER_STRINGCOPY(CTESTER_FAILURE_LINE, CTESTER_STRINGIFY(__LINE__));\
    CTESTER_FAILURE_CODE = CODE;\
    return CODE;\
} while (0)
#define CTesterBiassertionFailure(ACTUAL,EXPECTED,MESSAGE,CODE)\
do {\
    CTESTER_STRINGCOPY(CTESTER_FAILURE_ACTUAL, CTESTER_STRINGIFY(ACTUAL));\
    CTESTER_STRINGCOPY(CTESTER_FAILURE_OTHER, CTESTER_STRINGIFY(EXPECTED));\
    CTESTER_STRINGCOPY(CTESTER_FAILURE_MESSAGE, CTESTER_STRINGIFY(MESSAGE));\
    CTESTER_STRINGCOPY(CTESTER_FAILURE_FILE, __FILE__);\
    CTESTER_STRINGCOPY(CTESTER_FAILURE_LINE, CTESTER_STRINGIFY(__LINE__));\
    CTESTER_FAILURE_CODE = CODE;\
    return CODE;\
} while (0)

#define CTesterAssertionEqual(ACTUAL,EXPECTED,COMMENT) CTesterAssertionStateReset(); if ((ACTUAL) == (EXPECTED)) CTesterAssertionSuccess(); else CTesterBiassertionFailure(Acutal,EXPECTED,AssertEqual(ACTUAL, EXPECTED),-1)
#define CTesterAssertionNotEqual(ACTUAL,EXPECTED,COMMENT) CTesterAssertionStateReset(); if ((ACTUAL) != (EXPECTED)) CTesterAssertionSuccess(); else CTesterBiassertionFailure(Acutal,EXPECTED,AssertNotEqual(ACTUAL, EXPECTED),-1)
#define CTesterAssertionDeepEqual(ACTUAL,EXPECTED,COMPARE,COMMENT) CTesterAssertionStateReset(); if (COMPARE((ACTUAL), (EXPECTED)) == 0) CTesterAssertionSuccess(); else CTesterBiassertionFailure(Acutal,EXPECTED,AssertDeepEqual(ACTUAL, EXPECTED, COMPARE),-1)
#define CTesterAssertionTrue(ACTUAL,COMMENT) CTesterAssertionStateReset(); if (ACTUAL) CTesterAssertionSuccess(); else CTesterAssertionFailure(Acutal,AssertTrue(ACTUAL),-1)
#define CTesterAssertionFalse(ACTUAL,COMMENT) CTesterAssertionStateReset(); if (!(ACTUAL)) CTesterAssertionSuccess(); else CTesterAssertionFailure(Acutal,AssertFalse(ACTUAL),-1)
#define CTesterAssertionLessThan(VALUE,LIMIT,COMMENT) CTesterAssertionStateReset(); if ((VALUE) < (LIMIT)) CTesterAssertionSuccess(); else CTesterBiassertionFailure(VALUE,LIMIT,AssertLessThan(VALUE, LIMIT),-1)
#define CTesterAssertionLessThanOrEqual(VALUE,LIMIT,COMMENT) CTesterAssertionStateReset(); if ((VALUE) <= (LIMIT)) CTesterAssertionSuccess(); else CTesterBiassertionFailure(VALUE,LIMIT,AssertLessThanOrEqual(VALUE, LIMIT),-1)
#define CTesterAssertionGreaterThan(VALUE,LIMIT,COMMENT) CTesterAssertionStateReset(); if ((VALUE) > (LIMIT)) CTesterAssertionSuccess(); else CTesterBiassertionFailure(VALUE,LIMIT,AssertGreaterThan(VALUE, LIMIT),-1)
#define CTesterAssertionGreaterThanOrEqual(VALUE,LIMIT,COMMENT) CTesterAssertionStateReset(); if ((VALUE) >= (LIMIT)) CTesterAssertionSuccess(); else CTesterBiassertionFailure(VALUE,LIMIT,AssertGreaterThanOrEqual(VALUE, LIMIT),-1)

#define AssertEqual(ACTUAL, EXPECTED) CTesterAssertionEqual(ACTUAL, EXPECTED, "Assert ACTUAL and EXPECTED are equal '=='")
#define AssertNotEqual(ACTUAL, EXPECTED) CTesterAssertionNotEqual(ACTUAL, EXPECTED, "Assert ACTUAL and EXPECTED are not equal '!='")
#define AssertDeepEqual(ACTUAL, EXPECTED, COMPARE) CTesterAssertionDeepEqual(ACTUAL, EXPECTED, COMPARE, "Assert ACTUAL and EXPECTED are deep-equal '=='")
#define AssertTrue(ACTUAL) CTesterAssertionTrue(ACTUAL, "Assert ACTUAL is true 'if (ACTUAL)'")
#define AssertFalse(ACTUAL) CTesterAssertionFalse(ACTUAL, "Assert ACTUAL is false 'if (!ACTUAL)'")
#define AssertLessThan(VALUE, LIMIT) CTesterAssertionLessThan(VALUE, LIMIT, "Assert VALUE is less than LIMIT '<'")
#define AssertLessThanOrEqual(VALUE, LIMIT) CTesterAssertionLessThanOrEqual(VALUE, LIMIT, "Assert VALUE is less than or equal to LIMIT '<='")
#define AssertGreaterThan(VALUE, LIMIT) CTesterAssertionGreaterThan(VALUE, LIMIT, "Assert VALUE is greater than LIMIT '>'")
#define AssertGreaterThanOrEqual(VALUE, LIMIT) CTesterAssertionGreaterThanOrEqual(VALUE, LIMIT, "Assert VALUE is greater than or equal to LIMIT '>='")

#define CTesterAssertionEqualDelta(ACTUAL,EXPECTED,EPSILON,COMMENT) CTesterAssertionStateReset(); if (((ACTUAL) - (EXPECTED)) <= (EPSILON) && ((EXPECTED) - (ACTUAL)) <= (EPSILON)) CTesterAssertionSuccess(); else CTesterBiassertionFailure(ACTUAL,EXPECTED,AssertEqualDelta(ACTUAL, EXPECTED, EPSILON),-1)
#define CTesterAssertionNotEqualDelta(ACTUAL,EXPECTED,EPSILON,COMMENT) CTesterAssertionStateReset(); if (((ACTUAL) - (EXPECTED)) <= (EPSILON) && ((EXPECTED) - (ACTUAL)) <= (EPSILON)) CTesterBiassertionFailure(ACTUAL,EXPECTED,AssertNotEqualDelta(ACTUAL, EXPECTED, EPSILON),-1); else CTesterAssertionSuccess()

#define AssertEqualDelta(ACTUAL, EXPECTED, EPSILON) CTesterAssertionEqualDelta(ACTUAL, EXPECTED, EPSILON, "Assert ACTUAL and EXPECTED are equal within Delta '|ACTUAL - EXPECTED| <= EPSILON'")
#define AssertNotEqualDelta(ACTUAL, EXPECTED, EPSILON) CTesterAssertionNotEqualDelta(ACTUAL, EXPECTED, EPSILON, "Assert ACTUAL and EXPECTED are not equal within Delta '|ACTUAL - EXPECTED| > EPSILON'")

#define CTesterAssertionArrayEqual(ACTUAL, EXPECTED, LENGTH) do {\
    CTesterAssertionStateReset();\
    CTESTER_ARRAY_TEST = 1;\
    while (++CTESTER_ARRAY_TEST_INDEX < LENGTH) {\
        if ((ACTUAL)[CTESTER_ARRAY_TEST_INDEX] == (EXPECTED)[CTESTER_ARRAY_TEST_INDEX])\
            CTesterAssertionNoOp();\
        else\
            CTesterBiassertionFailure((ACTUAL)[CTESTER_ARRAY_TEST_INDEX],(EXPECTED)[CTESTER_ARRAY_TEST_INDEX],AssertArrayEqual(ACTUAL, EXPECTED, LENGTH),-1);\
    }\
    CTesterAssertionSuccess();\
} while (0)
#define AssertArrayEqual(ACTUAL, EXPECTED, LENGTH) CTesterAssertionArrayEqual(ACTUAL, EXPECTED, LENGTH)

#define CTesterAssertionArrayDeepEqual(ACTUAL, EXPECTED, LENGTH, COMPARE) do {\
    CTesterAssertionStateReset();\
    CTESTER_ARRAY_TEST = 1;\
    while (++CTESTER_ARRAY_TEST_INDEX < LENGTH) {\
        if ((COMPARE((ACTUAL)[CTESTER_ARRAY_TEST_INDEX], (EXPECTED)[CTESTER_ARRAY_TEST_INDEX])) == 0)\
            CTesterAssertionNoOp();\
        else\
            CTesterBiassertionFailure((ACTUAL)[CTESTER_ARRAY_TEST_INDEX],(EXPECTED)[CTESTER_ARRAY_TEST_INDEX],AssertArrayDeepEqual(ACTUAL, EXPECTED, LENGTH, COMPARE),-1);\
    }\
    CTesterAssertionSuccess();\
} while (0)
#define AssertArrayDeepEqual(ACTUAL, EXPECTED, LENGTH, COMPARE) CTesterAssertionArrayDeepEqual(ACTUAL, EXPECTED, LENGTH, COMPARE)

#define CTesterAssertionStringEqual(ACTUAL, EXPECTED) do {\
    CTesterAssertionStateReset();\
    CTESTER_ARRAY_TEST = 1;\
    while (1) {\
        CTESTER_ARRAY_TEST_INDEX++;\
        if ((ACTUAL)[CTESTER_ARRAY_TEST_INDEX] == (EXPECTED)[CTESTER_ARRAY_TEST_INDEX])\
            CTesterAssertionNoOp();\
        else\
            CTesterBiassertionFailure((ACTUAL)[CTESTER_ARRAY_TEST_INDEX],(EXPECTED)[CTESTER_ARRAY_TEST_INDEX],AssertStringEqual(ACTUAL, EXPECTED),-1);\
        if ((ACTUAL)[CTESTER_ARRAY_TEST_INDEX] == 0)\
            break;\
    }\
    CTesterAssertionSuccess();\
} while (0)
#define AssertStringEqual(ACTUAL, EXPECTED) CTesterAssertionStringEqual(ACTUAL, EXPECTED)

#endif