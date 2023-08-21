#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <cstring>

/*
Some words on race conditions:
I don't really care about race conditions... If two threads write to one
index in the array at the same time, they will just be writing the same
value anyway. And if a thread attempts to read from an index in the array
while another is writing, that doesn't matter as either it just gets a 0
and calculates what the other thread has already calculated.

Some words on performance:
Spawning threads with the -O3 flag doesn't seem to generate much overhead,
so this solution is faster than the non-parallelized one (at least on ARM64).
The number of threads should probably match the number of cores or hyper-
threads available on the CPU. You should also experiment with multiples of
number of cores. Have fun!
*/

void collatzThread(int64_t from, int64_t to, uint16_t threadID);
void collatzThreadRec(int64_t from, int64_t to, uint16_t threadID);
int64_t collatzNext(int64_t x, uint8_t& shiftcounter);
int64_t collatzRec(int64_t x);
int64_t collatz(int64_t x);
inline uint8_t countTrailingZeros(int64_t x);
void printMemoArray();

int64_t *memo;
int64_t N;
uint16_t nThreads;
bool recursive = false;
void (*thrFunc)(int64_t, int64_t, uint16_t) = &collatzThread;


int main(int argc, char **argv) {
    // argv check
    if (argc < 2) {
        std::cout << "Program takes two arguments: N and nThreads" << std::endl;
        std::cout << "If you add 'rec' as third argument, you will run the"
                     << " experimental recursive version." << std::endl;
        std::cout << "If only one parameter is given, nThreads will default to 4" << std::endl;
        std::cout << "Either way, you cannot run the program with 0 args..." << std::endl;
        return 1;
    }
    else if (argc == 2) {
        nThreads = 4;
    }
    else if (argc >= 3) {
        nThreads = atoi(argv[2]);
    }
    if (strcmp("rec", argv[argc-1]) == 0) {
        recursive = true;
        thrFunc = &collatzThreadRec;
    }

    // Set N and calculate number of passes per thread
    N = (int64_t) atoi(argv[1]) + 1;
    if (N < nThreads) {
        std::cout << "Error: you specified nThreads > N. Aborting." << std::endl;
        return 2;
    }
    int64_t numPerThread = N / nThreads;
    

    // Set up memoization array
    memo = new int64_t[N];
    for (int64_t i = 0; i < N; i++) {
        memo[i] = 0;
    }

    // Set up thread vector
    std::vector<std::thread> threads;

    // Print startup info
    std::cout << "Configuration: N = " << N-1 << ", nThreads = " << nThreads << std::endl;
    if (recursive) {
        std::cout << "Recursive algorithm selected" << std::endl;
    }
    else {
        std::cout << "Iterative algorithm selected" << std::endl;
    }
    std::cout << "" << std::endl;


    // Timer and main loop
    auto start = std::chrono::high_resolution_clock::now();
    for(uint16_t i = 0; i < nThreads-1; i++) {
        uint64_t from = (i != 0) ? (i*numPerThread) : 2;
        uint64_t to = from + numPerThread - 1;
        threads.push_back(std::thread(thrFunc, from, to, i));
    }
    uint64_t lastFrom = nThreads != 1 ? (nThreads-1)*numPerThread : 2;
    uint64_t lastTo = N-1;
    threads.push_back(std::thread(thrFunc, lastFrom, lastTo, nThreads));

    for (uint16_t i = 0; i < threads.size(); i++) {
        threads[i].join();
    }
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate exec time and print
    auto exectime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time to run " << N-1 << " numbers: " << exectime.count() << " ms" << std::endl;

    #ifdef PRINT_MEMO_ARRAY
    printMemoArray();
    #endif

    return 0;
}

void collatzThread(int64_t from, int64_t to, uint16_t threadID) {
    for (int64_t i = from; i <= to; i++) {
        collatz(i);
    }
}

void collatzThreadRec(int64_t from, int64_t to, uint16_t threadID) {
    for (int64_t i = from; i <= to; i++) {
        collatzRec(i);
    }
}

int64_t collatz(int64_t x) {
    uint8_t shiftcounter = 0;
    int64_t pathLen = 0;
    int64_t nextX = collatzNext(x, shiftcounter);
    pathLen += shiftcounter;
    while (nextX != 1) {
        if (nextX < N && memo[nextX] != 0) {
            memo[x] = pathLen + memo[nextX];
            return pathLen + memo[nextX];
        }
        nextX = collatzNext(nextX, shiftcounter);
        pathLen += shiftcounter;
    }
    memo[x] = pathLen;
    return pathLen;
}

int64_t collatzNext(int64_t x, uint8_t& shiftcounter) {
    uint8_t trailingZeros = countTrailingZeros(x);
    if (x % 2 == 0) {
        shiftcounter = trailingZeros;
        return x >> trailingZeros;
    }
    shiftcounter = 1;
    return ((x << 1) | 1) + x;
}

int64_t collatzRec(int64_t x) {
    if (x == 1) {
        return 0;
    }
    if (x < N && memo[x] != 0) {
        return memo[x];
    }
    int64_t currPathLen;
    if (x % 2 == 0) {
        uint8_t trailingZeros = countTrailingZeros(x);
        currPathLen = collatzRec(x >> trailingZeros) + (int64_t)trailingZeros;
    }
    else {
        currPathLen = collatzRec(((x << 1) | 1) + x) + 1;
    }
    if (x < N) {
        memo[x] = currPathLen;
    }
    return currPathLen;
}

inline uint8_t countTrailingZeros(int64_t x) {
    #ifdef _WIN64
    return (uint8_t)__lzcnt64(x);
    #else
    return (uint8_t)__builtin_ctz(x);
    #endif
}

void printMemoArray() {
    for (uint64_t i = 0; i < N; i++) {
        std::cout << i << ": " << memo[i] << std::endl;
    }
}