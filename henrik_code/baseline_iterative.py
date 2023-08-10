import time

def collatz_next(x):
	if x % 2 == 0:
		return x//2
	else:
		return x*3 + 1

def collatz(x):
	path_len = 0
	next_x = collatz_next(x)
	while next_x != 1:
		path_len += 1
		next_x = collatz_next(next_x)
	return path_len + 1


if __name__ == "__main__":
	N = 1_000_000 # I wouldn't dare run this with 1*10**7
	start = time.time()
	for i in range(2, N):
		collatz(i)
	run_time = time.time() - start
	print("Time to run", N, "numbers was:", round(run_time, 3), "seconds.")
