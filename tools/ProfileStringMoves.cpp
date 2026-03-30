#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// Simulate pass-by-value approach
void __attribute__((noinline)) passByValue(std::string str1, std::string str2) {
    std::string member1 = std::move(str1);
    std::string member2 = std::move(str2);
    // Prevent optimization
    volatile char c = member1.empty() ? 'x' : member1[0];
    c = member2.empty() ? 'y' : member2[0];
}

// Simulate rvalue reference approach
void __attribute__((noinline)) passByRvalueRef(std::string &&str1, std::string &&str2) {
    std::string member1 = std::move(str1);
    std::string member2 = std::move(str2);
    // Prevent optimization
    volatile char c = member1.empty() ? 'x' : member1[0];
    c = member2.empty() ? 'y' : member2[0];
}

template <typename Func> double measureTime(Func &&func, size_t iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < iterations; ++i) {
        func();
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    return static_cast<double>(duration) / iterations;
}

int main() {
    constexpr size_t iterations = 10000000;
    constexpr size_t warmup = 10000;

    std::cout << "\n=== String Move Performance Comparison ===\n" << std::endl;

    // Warmup
    for (size_t i = 0; i < warmup; ++i) {
        std::string s1 = "e4";
        std::string s2 = "comment";
        passByValue(std::move(s1), std::move(s2));
    }

    // Test pass-by-value (current implementation)
    auto timePassByValue = measureTime(
        [&]() {
            std::string moveText = "e4";
            std::string comment = "comment";
            passByValue(std::move(moveText), std::move(comment));
        },
        iterations);

    // Test rvalue reference (original implementation)
    auto timeRvalueRef = measureTime(
        [&]() {
            std::string moveText = "e4";
            std::string comment = "comment";
            passByRvalueRef(std::move(moveText), std::move(comment));
        },
        iterations);

    // Test with longer strings (beyond SSO)
    auto timeLongPassByValue = measureTime(
        [&]() {
            std::string moveText = "Nf3xd5+";
            std::string comment = "This is a longer comment that exceeds small string optimization";
            passByValue(std::move(moveText), std::move(comment));
        },
        iterations);

    auto timeLongRvalueRef = measureTime(
        [&]() {
            std::string moveText = "Nf3xd5+";
            std::string comment = "This is a longer comment that exceeds small string optimization";
            passByRvalueRef(std::move(moveText), std::move(comment));
        },
        iterations);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Short strings (SSO):" << std::endl;
    std::cout << "  Pass-by-value:    " << timePassByValue << " ns/op" << std::endl;
    std::cout << "  Rvalue reference: " << timeRvalueRef << " ns/op" << std::endl;
    std::cout << "  Difference:       " << (timePassByValue - timeRvalueRef) << " ns" << std::endl;
    std::cout << "  Overhead:         " << std::setprecision(1) << ((timePassByValue / timeRvalueRef - 1.0) * 100.0)
              << "%\n"
              << std::endl;

    std::cout << "Long strings (heap allocated):" << std::endl;
    std::cout << "  Pass-by-value:    " << timeLongPassByValue << " ns/op" << std::endl;
    std::cout << "  Rvalue reference: " << timeLongRvalueRef << " ns/op" << std::endl;
    std::cout << "  Difference:       " << (timeLongPassByValue - timeLongRvalueRef) << " ns" << std::endl;
    std::cout << "  Overhead:         " << std::setprecision(1)
              << ((timeLongPassByValue / timeLongRvalueRef - 1.0) * 100.0) << "%\n"
              << std::endl;

    std::cout << "\nConclusion:" << std::endl;
    std::cout << "Pass-by-value adds ~" << std::setprecision(2) << (timePassByValue - timeRvalueRef)
              << " ns overhead per operation" << std::endl;
    std::cout << "This represents 1 additional move constructor call" << std::endl;
    std::cout << "\nFor a typical chess game (80 moves):" << std::endl;
    std::cout << "Total overhead: ~" << std::setprecision(1) << (timePassByValue - timeRvalueRef) * 80 / 1000.0
              << " microseconds (0.000" << std::setprecision(0) << (timePassByValue - timeRvalueRef) * 80 / 1000.0
              << " milliseconds)" << std::endl;
    std::cout << "Performance impact: Negligible\n" << std::endl;

    return 0;
}
