
#include "test_one.h"
#include "test_xyz.h"
#include "tester.h"

int main(void) {
    AddSuite(TestOne);
    AddSuite(TestXYZ);

    RunTests();
}
