fn next_collatz(n: usize) -> usize {
    if n % 2 == 0 {
        n / 2
    } else {
        3 * n + 1
    }
}

fn collatz(n: usize) -> u16 {
    let mut memo: Vec<u16> = vec![0; n + 1];
    let mut path: Vec<usize> = Vec::with_capacity(1000);
    memo[1] = 1;

    for x in 1..n {
        if memo[x] > 0 {
            continue;
        }
        path.clear();
        path.push(x);
        let mut offset = 0;

        while let Some(&p) = path.last() {
            if p <= n {
                offset = memo[p];
                if offset > 0 {
                    break;
                }
            }
            path.push(next_collatz(p));
        }
        path.iter().rev().enumerate().filter(|&(_, y)| *y <= n).for_each(|(i, &y)| {
            memo[y] = offset + i as u16;
        });

    }
    *memo.iter().max().unwrap()
}

fn main() {
    let n = 1E9 as usize;
    println!("{}", collatz(n));
}
