from decimal import Decimal
from sys import argv
import time


"""
This solution uses memoization utilizing Python dictionaries. Apart from that, it's basically a stupid recursive solver.
I have no recollection of why I put it into a class, but there you have it.

Feel free to play with the plotting, I cannot really remember how it works, and it should probably have been toggled
by args instead of in the code directly.
"""

class Collatz:
    def __init__(self):
        self.checked = {1: 0}
        self.current_check_value = 0

    def collatz(self, n, i):
        if self.current_check_value == 0:
            self.current_check_value = n
        if n in self.checked:
            self.checked[self.current_check_value] = i + self.checked[n]
            self.current_check_value = 0
            return i + self.checked[n]
        if n % 2 == 0:
            n = n / 2
        else:
            n = (n * 3) + 1
        return self.collatz(n, i+1)

if __name__ == "__main__":
    # Make singleton object
    c = Collatz()

    # Check argv and set N
    try:
        N = int(argv[1])
    except:
        print("Program takes exactly ONE parameter: N")
        quit(1)

    # Timer and main loop
    start = time.time()
    for n in range(1, N):
        c.collatz(n, 0)
    end = time.time()-start

    # Calculate exec time and print
    exectime = int(round(end, 3) * 1000)
    print("Time to run", N, "numbers:", exectime, "ms")
    
    # Plot stuff, just go for False...
    plot = False
    if plot:
        import matplotlib.pyplot as plt
        plt.scatter(list(c.checked.keys()), list(c.checked.values()), s=0.2)
        plt.grid()
        plt.axhline(0, color="black")
        plt.axvline(0, color="black")
        plt.show()
