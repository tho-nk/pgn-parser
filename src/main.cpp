#include <concepts>
#include <iostream>

std::integral auto factorial(std::integral auto a) {
    if (a <= 0)
        return 1;
    else
        return a * factorial(a - 1);
}

int main() {
    std::cout << factorial(10) << std::endl;
    return 0;
}
