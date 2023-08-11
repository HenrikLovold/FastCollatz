#include <string>
#include <iostream>
#include <chrono>

int64_t collatzNext(int64_t x);
int64_t collatz(int64_t x);

int64_t *memo;
int64_t N;

int main(int argc, char **argv) {
    // argv check and set N
    if (argc != 2) {
        std::cout << "Program takes exactly ONE parameter: N" << std::endl;
        return 1;
    }
    N = (int64_t) atoi(argv[1]) + 1;
    
    // Set up memoization array
    memo = new int64_t[N];
    for (int64_t i; i < N; i++) {
        memo[i] = 0;
    }

    // Timer and main loop
    auto start = std::chrono::high_resolution_clock::now();
    for (int64_t i = 2; i < N; i++) {
        collatz(i);
    }
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate exec time and print
    auto exectime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time to run " << N-1 << " numbers: " << exectime.count() << " ms" << std::endl;

    return 0;
}

int64_t collatz(int64_t x) {
    int64_t pathLen = 0;
    int64_t nextX = collatzNext(x);
    while (nextX != 1) {
        if (nextX < N && memo[nextX] != 0) {
            memo[x] = pathLen + memo[nextX] + 1;
            return pathLen + memo[nextX] + 1;
        }
        pathLen++;
        nextX = collatzNext(nextX);
    }
    memo[x] = pathLen+1;
    return pathLen+1;
}

int64_t collatzNext(int64_t x) {
    if (x % 2 == 0) {
        return x >> 1;
    }
    return (x << 1) + x + 1;
}