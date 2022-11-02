#include "testDefines.h"

int main() {
    TEST_GROUP_BEGIN("check")
    TEST_CHECK(true)
    TEST_CHECK(false)
    TEST_GROUP_END

    TEST_GROUP_BEGIN("compare")
    TEST_CHECK(1 + 1 == 2)
    TEST_CHECK(1 * 1 == 2)
    TEST_GROUP_END
}
