#include <benchmark/benchmark.h>
#include <string>
#include <utility> // for std::move

// Pass-by-value function
void passByValue(std::string str1, std::string str2) {
    std::string member1 = std::move(str1);
    std::string member2 = std::move(str2);
    volatile char c = member1.empty() ? 'x' : member1[0];
    c = member2.empty() ? 'y' : member2[0];
}

// Pass-by-rvalue-reference function
void passByRvalueRef(std::string &&str1, std::string &&str2) {
    std::string member1 = std::move(str1);
    std::string member2 = std::move(str2);
    volatile char c = member1.empty() ? 'x' : member1[0];
    c = member2.empty() ? 'y' : member2[0];
}

// ---------------- Benchmarks ----------------

// Short strings, pass by value
static void BM_PassByValueShort(benchmark::State &state) {
    for (auto _ : state) {
        passByValue(std::string("e4"), std::string("comment"));
        benchmark::DoNotOptimize(_);
    }
}
BENCHMARK(BM_PassByValueShort);

// Short strings, pass by rvalue ref
static void BM_PassByRvalueRefShort(benchmark::State &state) {
    for (auto _ : state) {
        passByRvalueRef(std::string("e4"), std::string("comment"));
        benchmark::DoNotOptimize(_);
    }
}
BENCHMARK(BM_PassByRvalueRefShort);

// Long strings, pass by value
static void BM_PassByValueLong(benchmark::State &state) {
    for (auto _ : state) {
        passByValue(std::string("Nf3xd5+"),
                    std::string("This is a longer comment that exceeds small string optimization"));
        benchmark::DoNotOptimize(_);
    }
}
BENCHMARK(BM_PassByValueLong);

// Long strings, pass by rvalue ref
static void BM_PassByRvalueRefLong(benchmark::State &state) {
    for (auto _ : state) {
        passByRvalueRef(std::string("Nf3xd5+"),
                        std::string("This is a longer comment that exceeds small string optimization"));
        benchmark::DoNotOptimize(_);
    }
}
BENCHMARK(BM_PassByRvalueRefLong);

// Single string move benchmark
static void BM_StringMove(benchmark::State &state) {
    for (auto _ : state) {
        std::string s = "e4";
        std::string moved = std::move(s);
        benchmark::DoNotOptimize(moved);
    }
}
BENCHMARK(BM_StringMove);
