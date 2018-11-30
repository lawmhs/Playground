use std::io;

fn main() {
    let mut input = String::new();

    io::stdin().read_line(&mut input)
        .ok()
        .expect("Couldn't read line");

    let input: i32 = input.trim().parse()
        .expect("Please type a number!");

    let res = fibonacci(input);

    println!("The result is {}", res);
}


fn fibonacci(n: i32)->i32 {

    let mut prev_num = 1;
    let mut curr_num = 1;
    for _i in 2..n { // underscored variables don't trigger anything in the compiler
        let temp = curr_num;
        curr_num = prev_num + curr_num;
        prev_num = temp; // fibonacci starts from n
    }
    return curr_num;
}