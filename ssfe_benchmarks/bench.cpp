#include <rte_common.h>
#include <rte_memory.h>
#include <rte_launch.h>
#include <rte_eal.h>
#include <rte_efd.h>
#include <rte_per_lcore.h>
#include <rte_lcore.h>
#include <rte_cycles.h>
#include <rte_timer.h>
#include <rte_debug.h>

#include <cassert>
#include <chrono>
#include <map>

void init_rte_eal() {
    char *argv[3];
    char a0[] = "./bench";
    char a1[] = "--no-huge";
    char a2[] = "-m256"; // reserve 256 MB memory
    argv[0] = a0;
    argv[1] = a1;
    argv[2] = a2;
    int ret = rte_eal_init(3, argv);
    if (ret < 0)
        rte_panic("Cannot init EAL\n");
}

efd_value_t bench_lookup(struct rte_efd_table *table, int n) {
    printf("benchmaking lookup...\n");
    efd_value_t x = 0;
    auto start = std::chrono::high_resolution_clock::now();
    int cnt = 0;
    for (int round = 0; round < 10; ++round) {
        for (int i = 0; i < n; i++) {
            cnt++;
            uint64_t key = i;
            x ^= rte_efd_lookup(table, 0, &key);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    double ns = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / cnt;
    printf("time/lookup = %.3lf ns\n", ns);
    return x;
}

void bench_lookup_bulk(struct rte_efd_table *table, int n) {
    printf("benchmaking lookup_bulk...\n");
    uint64_t keys[16];
    void *key_list[16];
    for (int i = 0; i < 16; i++) {
        key_list[i] = (void *)&keys[i];
    }
    efd_value_t val[16];

    auto start = std::chrono::high_resolution_clock::now();
    int cnt = 0;
    for (int round = 0; round < 10; round++) {
        for (int i = 0; i < n - 16; i += 16) {
            cnt++;
            for (int j = 0; j < 16; j ++) {
                keys[j] = i + j;
            }
            rte_efd_lookup_bulk(table, 0, 16, (const void **)key_list, val);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto ns = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / cnt;
    printf("time/16_lookups = %.3lf ns\n", ns);

}

int main() {
    init_rte_eal();

    int n = 2000000;
    auto *table = rte_efd_create("hi", n, 64/8, 1, 1);
    assert(table != nullptr);

    for (int i = 0; i < n; i++) {
        uint64_t key = i;
        efd_value_t val = (uint8_t)i % 2;
        assert(rte_efd_update(table, 0, &key, val) == 0);
    }

    printf("\n");
    bench_lookup(table, n);
    bench_lookup_bulk(table, n);

    rte_efd_free(table);
    return 0;
}
