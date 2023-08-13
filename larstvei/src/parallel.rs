use argparse::{ArgumentParser, Store};
use rayon::prelude::*;
use std::sync::atomic::Ordering;
use std::sync::atomic::AtomicU16;

fn next_collatz(n: usize) -> usize {
    if n % 2 == 0 {
        n / 2
    } else {
        3 * n + 1
    }
}

fn collatz_rec(n: usize, memo: &[AtomicU16]) -> u16 {
    match memo.get(n).map(|x| x.load(Ordering::Relaxed)) {
        Some(0) => {
            let res = 1 + collatz_rec(next_collatz(n), memo);
            memo[n].store(res, Ordering::Relaxed);
            res
        }
        Some(y) => y,
        None => 1 + collatz_rec(next_collatz(n), memo),
    }
}

fn main() {
    let mut n = 1E9 as usize;

    {
        let mut parser = ArgumentParser::new();
        parser.set_description("Longest Collatz sequence starting below n");
        parser.refer(&mut n).add_argument("n", Store, "Value of n");
        parser.parse_args_or_exit();
    }

    let memo: Vec<AtomicU16> = (0..=n).map(|_| AtomicU16::new(0)).collect();
    memo[1].store(1, Ordering::Relaxed);
    (1..=n).into_par_iter().for_each(|x| { collatz_rec(x, &memo); });
    let res = memo.iter().map(|x| x.load(Ordering::Relaxed)).max().unwrap();
    println!("{}", res);
}
