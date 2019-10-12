cc_library(
    name = "dpdk",
    hdrs = glob(["x86_64-native-linux-gcc/include/**/*.h"]),
    strip_include_prefix = "x86_64-native-linux-gcc/include",
    srcs = glob(["x86_64-native-linux-gcc/lib/*.a"]),
    linkopts = ["-pthread", "-lnuma", "-ldl"],
)

cc_library(
    name = "sepset",
    hdrs = ["ssfe_benchmarks/sepset.h"],
    strip_include_prefix = "ssfe_benchmarks",
    deps = [
        ":dpdk"
    ]
)

COPTS = ["-std=c++17", "-O3", "-march=native", "-Wall", "-Wextra", "-Werror", "-Wno-error=register"]
LINKOPTS = ["-pthread", "-lnuma", "-ldl"]

cc_binary(
    name = "bench",
    srcs = ["ssfe_benchmarks/bench.cpp"],
    copts = ["-std=c++17", "-O3", "-march=native", "-Wall", "-Wextra", "-Werror", "-Wno-error=register"],
    linkopts = ["-pthread", "-lnuma", "-ldl"],
    deps = [
        ":sepset",
        "@benchmark//:benchmark", 
        "@benchmark//:benchmark_main",
    ]
)

cc_test(
    name = "test",
    srcs = glob(["ssfe_benchmarks/*_test.cpp"]),
    copts = COPTS,
    linkopts = LINKOPTS,
    deps = [
        "//:sepset",
        "@gtest//:gtest",
        "@gtest//:gtest_main",
    ]
)
