#include "sepset.h"

#include <chrono>
#include <map>

#include <benchmark/benchmark.h>

template <typename KEY_TYPE>
void insert_generated_kvs(SepSet<KEY_TYPE> &t, int n) {
    for (int i = 0; i < n; i++) {
        t.update(i, rand()%2);
    }
}

static void BM_sepset_update(benchmark::State& state) {
    auto t = SepSet<uint64_t>(state.range(0));
    int x = 0;
    for (int i = 0; i < state.range(0); i++) {
        t.update(i, (i%2)^x);
    }
    x ^= 1;

    // Benchmark
    int i = 0;
    for (auto _ : state) {
        t.update(i, (i%2)^x);
        i = i + 1;
        if (i == state.range(0)) {
            i = 0;
            x ^= 1;
        }
    }
}

static void BM_sepset_query(benchmark::State& state) {
    auto t = SepSet<uint64_t>(state.range(0));
    insert_generated_kvs(t, state.range(0));

    // Benchmark
    int i = 0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(t.query(i));
        i = i + 1;
        if (i == state.range(0)) {
            i = 0;
        }
    }
}


static void BM_sepset_build(benchmark::State& state) {
    auto t = SepSet<uint64_t>(state.range(0));

    // Benchmark
    int x = 0;
    for (auto _ : state) {
        auto t = SepSet<uint64_t>(state.range(0));
        for (int i = 0; i < state.range(0); i++) {
            t.update(i, (i%2)^x);
        }
        x^=0;
    }

    state.SetItemsProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
}

static void BM_sepset_query_batch(benchmark::State& state) {
    auto t = SepSet<uint64_t>(state.range(0));
    insert_generated_kvs(t, state.range(0));

    // Benchmark
    int i = 0;
    for (auto _ : state) {
        uint64_t keys[16];
        bool res[16];
        for (int j = 0; j < 16; ++j) {
            keys[j] = i;
            i += 1;
        }
        if (i + 16 >= state.range(0)) {
            i = 0;
        }
        t.query_batch(keys, res, 16);
        benchmark::DoNotOptimize(res[0]);
    }
}

BENCHMARK(BM_sepset_build)->Arg(10 * 1000)->Arg(100 * 1000)->Arg(1000 * 1000)->Arg(2 * 1000 * 1000);
BENCHMARK(BM_sepset_query)->Arg(10 * 1000)->Arg(100 * 1000)->Arg(1000 * 1000)->Arg(2 * 1000 * 1000);
BENCHMARK(BM_sepset_query_batch)->Arg(10 * 1000)->Arg(100 * 1000)->Arg(1000 * 1000)->Arg(2 * 1000 * 1000);
BENCHMARK(BM_sepset_update)->Arg(10 * 1000)->Arg(100 * 1000)->Arg(1000 * 1000)->Arg(2 * 1000 * 1000);