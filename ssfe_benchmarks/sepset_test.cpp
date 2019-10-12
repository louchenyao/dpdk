#include "sepset.h"

#include <gtest/gtest.h>


TEST(SepSet, Basic) {
    SepSet<uint64_t> s(100);
    s.update(2333, false);
    s.update(233, true);
    EXPECT_EQ(s.query(233), true);
    EXPECT_EQ(s.query(2333), false);
}