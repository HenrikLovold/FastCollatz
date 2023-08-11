# Henrik's workspace

Welcome! Here are some of my solutions to this problem, some in Python and some in C++. Currently, my fastest runs are achieved with the `multithreaded_bitshift.cpp` solution.

### Files

#### C++
- baseline_iterative.cpp
  - Straightforward solution
- iterative_memoized_normal.cpp
  - Pretty much the same, but using memoization in form of an array with size N
- iterative_memoized_bitshift.cpp
  - Same as the previous, with some bitshifting magic instead of mathematical operators
- multithreaded_bitshift.cpp
  - Currently the fastest solution, it's basically all in one package with some multithreading thrown in

#### Python
- baseline_iterative.py
  - Your simple straightforward solution
- class_based_memo_collatz.py
  - Class based iterative approach using memoization

### Compilation of C++ files
All files are header-less, and are really simple to compile. I use llvm-g++ on my ARM Mac, with the -O3 optimizer. Using this optimization level has not yielded any problems that I am aware of, but in case of trouble try with -O2 or even no optimization at all.

Be aware that I use some C++20 tricks, so you have to use a compiler supporting the C++20 standard.

This is the command I use:

`llvm-g++ <filename>.cpp -std=c++20 -O3 -o <program-name>`