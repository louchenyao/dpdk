#include "setsep.h"

#include <gtest/gtest.h>


TEST(SetSep, Basic) {
    SetSep<uint64_t> s(100);
    s.update(2333, false);
    s.update(233, true);
    EXPECT_EQ(s.query(233), true);
    EXPECT_EQ(s.query(2333), false);
}

TEST(SetSep, BuildWithTightSize) {
    int target_chunk_num_rules = EFD_TARGET_GROUP_NUM_RULES * 64; // This marco is defined in rte_efd.c, so we cannot include it.
    printf("EFD_TARGET_GROUP_NUM_RULES = %d\n", (int)EFD_TARGET_GROUP_NUM_RULES);
    printf("EFD_TARGET_CHUNK_NUM_RULES = %d\n", target_chunk_num_rules);
    for (int multi = 64; multi <= 8192; multi *= 2) {
        int size = target_chunk_num_rules * multi;
        printf("Building with size %d\n", size);
        SetSep<uint64_t> s(size);
        for (int i = 0; i < size; i++) {
            s.update(i, rand()%2);
        }
    }
}