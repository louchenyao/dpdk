#include "sepset.h"

#include <chrono>
#include <map>

template <typename KEY_TYPE>
void insert_generated_kvs(SepSet<KEY_TYPE> &t, int n) {
    for (int i = 0; i < n; i++) {
        t.update(i, rand()%2);
    }
}

efd_value_t bench_lookup(int n) {
    printf("benchmaking lookup...\n");
    auto t = SepSet<uint64_t>(n);
    insert_generated_kvs(t, n);

    efd_value_t x = 0;
    auto start = std::chrono::high_resolution_clock::now();
    int cnt = 0;
    for (int round = 0; round < 10; ++round) {
        for (int i = 0; i < n; i++) {
            cnt++;
            x ^= t.query(i);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    double ns =
        double(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
                   .count()) /
        cnt;
    printf("time/lookup = %.3lf ns\n", ns);
    return x;
}

void bench_lookup_bulk(int n) {
    printf("benchmaking lookup_bulk...\n");
    auto t = SepSet<uint64_t>(n);
    insert_generated_kvs(t, n);

    uint64_t keys[16];
    bool val[16];

    auto start = std::chrono::high_resolution_clock::now();
    int cnt = 0;
    for (int round = 0; round < 10; round++) {
        for (int i = 0; i < n - 16; i += 16) {
            cnt++;
            for (int j = 0; j < 16; j++) {
                keys[j] = i + j;
            }
            t.query_batch(keys, val, 16);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto ns =
        double(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
                   .count()) /
        cnt;
    printf("time/16_lookups = %.3lf ns\n", ns);
}

int main() {
    int n = 2000000;
    bench_lookup(n);
    bench_lookup_bulk(n);
    return 0;
}
