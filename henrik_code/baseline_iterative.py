import time
from sys import argv

def collatz_next(x):
	if x % 2 == 0:
		return x//2
	return x*3 + 1

def collatz(x):
	path_len = 0
	next_x = collatz_next(x)
	while next_x != 1:
		path_len += 1
		next_x = collatz_next(next_x)
	return path_len + 1


if __name__ == "__main__":
	# Check argv and set N
	try:
		N = int(argv[1])
	except:
		print("Program takes exactly ONE parameter: N")
		quit(1)

	# Timer and main loop
	start = time.time()
	for i in range(2, N+1):
		collatz(i)
	run_time = time.time() - start
	
	print("Time to run", N, "numbers:", int(round(run_time, 3)*1000), "ms")
