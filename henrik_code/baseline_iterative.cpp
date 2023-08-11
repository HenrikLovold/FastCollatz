#include <string>
#include <iostream>
#include <chrono>

int64_t collatzNext(int64_t x);
int64_t collatz(int64_t x);

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Program takes exactly ONE parameter: N" << std::endl;
        return 1;
    }
    int64_t N = (int64_t) atoi(argv[1]);

    auto start = std::chrono::high_resolution_clock::now();
    for (int64_t i = 2; i < N+1; i++) {
        collatz(i);
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto exectime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Time to run " << N << " numbers: " << exectime.count() << " ms" << std::endl;

    return 0;
}

int64_t collatz(int64_t x) {
    int64_t pathLen = 0;
    int64_t nextX = collatzNext(x);
    while (nextX != 1) {
        pathLen++;
        nextX = collatzNext(nextX);
    }
    return pathLen+1;
}

int64_t collatzNext(int64_t x) {
    if (x % 2 == 0) {
        return x / 2;
    }
    return x*3 + 1;
}