#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#define THREAD_VERBOSE
#undef THREAD_VERBOSE

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

void collatzThread(int64_t from, int64_t to, unsigned int threadID);
int64_t collatzNext(int64_t x, int& shiftcounter);
int64_t collatz(int64_t x);

int64_t *memo;
int64_t N;
unsigned int nThreads;

int main(int argc, char **argv) {
    // argv check
    if (argc < 2) {
        std::cout << "Program takes two parameters: N and nThreads" << std::endl;
        std::cout << "If only one parameter is given, nThreads will default to 4" << std::endl;
        std::cout << "Either way, you cannot run the program with 0 args..." << std::endl;
        return 1;
    }
    else if (argc == 2) {
        nThreads = 4;
    }
    else {
        nThreads = atoi(argv[2]);
    }

    // Set N and calculate number of passes per thread
    N = (int64_t) atoi(argv[1]) + 1;
    int64_t numPerThread = N / nThreads;
    

    // Set up memoization array
    memo = new int64_t[N];
    for (int64_t i; i < N; i++) {
        memo[i] = 0;
    }

    // Set up thread vector
    std::vector<std::thread> threads;

    // Timer and main loop
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < nThreads-1; i++) {
        int from = (i != 0) ? (i*numPerThread) : 2;
        int to = from + numPerThread - 1;
        threads.push_back(std::thread(collatzThread, from, to, i));
    }
    int lastFrom = nThreads != 1 ? (nThreads-1)*numPerThread : 2;
    int lastTo = N-1;
    threads.push_back(std::thread(collatzThread, lastFrom, lastTo, nThreads));

    for (int i = 0; i < threads.size(); i++) {
        threads[i].join();
    }
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate exec time and print
    auto exectime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time to run " << N-1 << " numbers: " << exectime.count() << " ms" << std::endl;

    return 0;
}

void collatzThread(int64_t from, int64_t to, unsigned int threadID) {
    #ifdef THREAD_VERBOSE
    std::cout << "Thread #" << threadID << " started" << std::endl;
    #endif
    for (int64_t i = from; i <= to; i++) {
        collatz(i);
    }
    #ifdef THREAD_VERBOSE
    std::cout << "Thread #" << threadID << " finished" << std::endl;
    #endif
}

int64_t collatz(int64_t x) {
    int shiftcounter = 0;
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

#ifndef _WIN64
int64_t collatzNext(int64_t x, int& shiftcounter) {
    uint8_t trailingZeros = (unsigned)__builtin_ctz(x);
    if (x % 2 == 0) {
        shiftcounter = trailingZeros;
        return x >> trailingZeros;
    }
    shiftcounter = 1;
    return ((x << 1) | 1) + x;
}
#endif

#ifdef _WIN64
int64_t collatzNext(int64_t x, int& shiftcounter) {
    uint64_t trailingZeros = (unsigned)__lzcnt64(x);
    if (x % 2 == 0) {
        shiftcounter = trailingZeros;
        return x >> trailingZeros;
    }
    shiftcounter = 1;
    return ((x << 1) | 1) + x;
}
#endif