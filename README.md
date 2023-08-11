# FastCollatz
Can you make it fast?

#### What is Collatz' conjecture?
See [Wikipedia](https://en.wikipedia.org/wiki/Collatz_conjecture).

#### What is the challenge?
Compute the length of all Collatz paths for numbers 2 to N as fast as possible. Some example lengths:

| Number  | Path length  |
|---|---|
| 2  | 1  |
| 3  | 7  |
| 4  | 3  |
| 5  | 5  |
| 6  | 8  |

If you need an example program, have a look at [this file](https://github.com/henrik2706/FastCollatz/blob/main/henrik_code/baseline_iterative.py).

#### Basic Collatz function
```python
collatz(x):
  if x % 2 == 0:
    return x / 2
  else:
    return x*3 + 1
```

#### Rules

- Calculate all Collatz paths for 2 to N
- Programs should take ONE argv parameter N

For testing purposes, run $N=1*10^7$, as this is what we're benchmarking.
