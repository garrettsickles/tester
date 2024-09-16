#ifndef CTESTER_TESTER_H
#define CTESTER_TESTER_H

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

#ifdef __cplusplus
#include <cstdio>
#define CTESTER_PRINTF(...) std::printf(__VA_ARGS__)
#else
#include <stdio.h>
#define CTESTER_PRINTF(...) printf(__VA_ARGS__)
#endif

#ifdef _MSC_VER
#include <Windows.h>
#define CTESTER_COLOR_RD 0x0C
#define CTESTER_COLOR_GN 0x0A
#define CTESTER_COLOR_CN 0x0B
#define CTESTER_COLOR_GY 0x07
#define CTESTER_COLOR_SET(CTESTER_PRINT_COLOR) do {\
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);\
    SetConsoleTextAttribute(hConsole, CTESTER_PRINT_COLOR);\
} while (0)
#else
#include <sys/ioctl.h>
#define CTESTER_COLOR_RD "\033[1m\033[31m"
#define CTESTER_COLOR_GN "\033[1m\033[32m"
#define CTESTER_COLOR_CN "\033[1m\033[36m"
#define CTESTER_COLOR_GY "\033[0m\033[37m"
#define CTESTER_COLOR_SET(TESTER_PRINT_COLOR) CTESTER_PRINTF(TESTER_PRINT_COLOR)
#endif

#define CTESTER_COLOR_PRINTF(CTESTER_PRINT_COLOR,...) do {\
    CTESTER_COLOR_SET(CTESTER_PRINT_COLOR);\
    CTESTER_PRINTF(__VA_ARGS__);\
    CTESTER_COLOR_SET(CTESTER_COLOR_GY);\
} while (0)
#define CTESTER_COLOR_RED_PRINTF(...) CTESTER_COLOR_PRINTF(CTESTER_COLOR_RD,__VA_ARGS__)
#define CTESTER_COLOR_GREEN_PRINTF(...) CTESTER_COLOR_PRINTF(CTESTER_COLOR_GN,__VA_ARGS__)
#define CTESTER_COLOR_CYAN_PRINTF(...) CTESTER_COLOR_PRINTF(CTESTER_COLOR_CN,__VA_ARGS__)
#define CTESTER_COLOR_GRAY_PRINTF(...) CTESTER_COLOR_PRINTF(CTESTER_COLOR_GY,__VA_ARGS__)

#ifndef CTESTER_TYPES_DECL
#define CTESTER_TYPES_DECL
typedef void (*CTEST_HELPER_TYPE)(void);
typedef int (*CTEST_FUNCTION_TYPE)(void);
struct CTesterExecutor
{
    const CTEST_HELPER_TYPE before;
    const CTEST_HELPER_TYPE after;
    const CTEST_FUNCTION_TYPE test;
    struct CTesterExecutor* prev;
    struct CTesterExecutor* next;
};
#endif

static struct CTesterExecutor CTESTER_INITIAL_TEST_INSTANCE = { .before = 0, .after = 0, .test = 0, .prev = 0, .next = 0 };
struct CTesterExecutor* CTESTER_CURRENT_TEST_INSTANCE = &CTESTER_INITIAL_TEST_INSTANCE;
long CTESTER_SUCCESS_ASSERTION_COUNTER;
long CTESTER_SUCCESS_INSTANCE_COUNTER = 0;
long CTESTER_FAILURE_INSTANCE_COUNTER = 0;
long CTESTER_CURRENT_INSTANCE_INDEX = 0;
long CTESTER_TOTAL_INSTANCE_COUNT = 0;
long CTESTER_ARRAY_TEST = 0;
long CTESTER_ARRAY_TEST_INDEX = 0;
long CTESTER_FAILURE_CODE = 0;
char CTESTER_CURRENT_SUITE_NAME[512];
char CTESTER_CURRENT_INSTANCE_NAME[512];
char CTESTER_FAILURE_ACTUAL[512];
char CTESTER_FAILURE_OTHER[512];
char CTESTER_FAILURE_MESSAGE[512];
char CTESTER_FAILURE_FILE[2048];
char CTESTER_FAILURE_LINE[16];

#define RunTests()\
do {\
    struct CTesterExecutor* instance = &CTESTER_INITIAL_TEST_INSTANCE; \
    while (instance) {\
        if (instance->test) CTESTER_TOTAL_INSTANCE_COUNT += 1;\
        instance = instance->next;\
    }\
    instance = &CTESTER_INITIAL_TEST_INSTANCE;\
    printf("[TEST] Tester Starting\n");\
    while (instance) {\
        if (instance->before) instance->before();\
        if (instance->test) {\
            CTESTER_COLOR_GRAY_PRINTF("[TEST]         %s::%s (%ld/%ld)\n", CTESTER_CURRENT_SUITE_NAME, CTESTER_CURRENT_INSTANCE_NAME, CTESTER_CURRENT_INSTANCE_INDEX + 1, CTESTER_TOTAL_INSTANCE_COUNT);\
            CTESTER_CURRENT_INSTANCE_INDEX += 1;\
            if (instance->test()) {\
                CTESTER_COLOR_GRAY_PRINTF("[TEST]                 ");  \
                if (CTESTER_ARRAY_TEST)\
                    CTESTER_COLOR_RED_PRINTF("FAILURE - %s at index %ld\n", CTESTER_FAILURE_MESSAGE, CTESTER_ARRAY_TEST_INDEX);\
                else\
                    CTESTER_COLOR_RED_PRINTF("FAILURE - %s\n", CTESTER_FAILURE_MESSAGE);\
                CTESTER_COLOR_GRAY_PRINTF("[TEST]                 %s:%s\n", CTESTER_FAILURE_FILE, CTESTER_FAILURE_LINE);\
                CTESTER_FAILURE_INSTANCE_COUNTER += 1;\
            } else {\
                CTESTER_SUCCESS_INSTANCE_COUNTER += 1;\
            }\
        }\
        if (instance->after) instance->after();\
        instance = instance->next;\
    }\
    CTESTER_COLOR_GRAY_PRINTF("[TEST] Tester Complete\n");\
    CTESTER_COLOR_GRAY_PRINTF("[TEST]     Results: Total %ld, Passed %ld, Failed %ld\n", CTESTER_TOTAL_INSTANCE_COUNT, CTESTER_SUCCESS_INSTANCE_COUNTER, CTESTER_FAILURE_INSTANCE_COUNTER);\
    return 0;\
} while(0)

#endif