# FastCollatz
Can you make it fast?

#### Basic Collatz function:
```
collatz(x):
  return x/2 if x%2 == 0
  return x*3 + 1 if x%2 == 1
```

#### Rules

- Calculate all Collatz paths for 2 to N+1
- Programs should take ONE argv parameter N

For testing purposes, run $N=1*10^7$
