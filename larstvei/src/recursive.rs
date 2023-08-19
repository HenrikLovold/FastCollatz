fn next_collatz(n: usize) -> usize {
    if n % 2 == 0 {
        n / 2
    } else {
        3 * n + 1
    }
}

fn collatz_rec(n: usize, memo: &mut [u16]) -> u16 {
    match memo.get(n) {
        Some(0) => {
            let res = 1 + collatz_rec(next_collatz(n), memo);
            memo[n] = res;
            res
        }
        Some(y) => *y,
        None => 1 + collatz_rec(next_collatz(n), memo),
    }
}

fn main() {
    let n = 1E9 as usize;
    let mut memo: Vec<u16> = vec![0; n];
    memo[1] = 1;
    (1..=n).for_each(|x| { collatz_rec(x, &mut memo); });
    let res = memo.iter().max().unwrap();
    println!("{}", res);
}
