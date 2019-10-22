#pragma once

#include <rte_common.h>
#include <rte_cycles.h>
#include <rte_debug.h>
#include <rte_eal.h>
#include <rte_efd.h>
#include <rte_launch.h>
#include <rte_lcore.h>
#include <rte_memory.h>
#include <rte_per_lcore.h>
#include <rte_timer.h>

#include <cassert>
#include <string>

void init_rte_eal() {
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    char *argv[3];
    char a0[] = "./bench";
    char a1[] = "--no-huge";
    char a2[] = "-m256";  // reserve 256 MB memory
    argv[0] = a0;
    argv[1] = a1;
    argv[2] = a2;
    int ret = rte_eal_init(3, argv);
    if (ret < 0) rte_panic("Cannot init EAL\n");
}

template <typename KEY_TYPE>
class SepSet {
   public:
    SepSet() = default;
    
    SepSet(int max_capacity) {
        init(max_capacity);
    }

    ~SepSet() {
        clear();
    }

    void init(int max_capacity) {
        assert(table_ == nullptr);
        init_rte_eal();
        table_ = rte_efd_create(("sepset_" + std::to_string(rand())).c_str(),
                                max_capacity, sizeof(KEY_TYPE), 1, 1);
        assert(table_ != nullptr);
    }

    void clear() {
        if (table_ != nullptr) {
            rte_efd_free(table_);
            table_ = nullptr;
        }
    }

    void build(const std::vector<std::pair<KEY_TYPE, bool>> &kvs) {
        for (const auto &kv : kvs) {
            update(kv.first, kv.second);
        }
    }

    inline void update(KEY_TYPE key, bool val) {
        assert(rte_efd_update(table_, 0, &key, (efd_value_t)val) == 0);
    }

    inline bool query(KEY_TYPE key) {
        return (bool)rte_efd_lookup(table_, 0, &key);
    }

    inline void query_batch(KEY_TYPE *keys, bool *res, int batch_size) {
        void *key_list[16];
        assert(batch_size <= 16);
        for (int i = 0; i < batch_size; i++) {
            key_list[i] = keys + i;
        }
        rte_efd_lookup_bulk(table_, 0, batch_size, (const void **)key_list, (efd_value_t *)res);
    }
   private:
    rte_efd_table *table_;
};
