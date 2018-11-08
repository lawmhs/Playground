extern crate rand; // extern for external dependencies

use std::io; // the things being used from our external dependencies or std lib
use std::cmp::Ordering;
use rand::Rng;

fn main() {
    println!("Guess the number!");

	let secret_number = rand::thread_rng().gen_range(1, 101); // inclusive lower, exclusive upper

	loop {

        println!("Please input your guess.");

        let mut guess = String::new(); // mut denotes that the variable is "mutable", ergo, you can change the variable value
// in essence, this allows you to express the intent of the variable, if you don't want it to be changed, set it without mut, if you want it to be changed, se twith mut
// by default, rust variables are immutable

        io::stdin().read_line(&mut guess)
            .expect("Failed to read line"); // don't make lines too long,
// having expect here makes it easier to read, two lines for two method calls
// associated functions are given by :: like in C++

        let guess: u32 = match guess.trim().parse() { // switching from expect to a match expression is how
            // we generally move on from crashing on error to handling on error
            Ok(num) => num,
            Err(_) => continue,
        }; // trim eliminates newlines \n, and parse eliminates leading and trailing whitespace
        // this is called "shadowing" where we overwrite the variable guess with another value, typically of a different type
// this allows us to reuse the guess variable name instead of needing to create two unique variables
// though this can cause errors in regularity, be careful!

        println!("You guessed: {}", guess);

        match guess.cmp(&secret_number) { // cmp means compare? somehow we are comparing a string and a number lol
            Ordering::Less => println!("Too small!"),
            Ordering::Greater => println!("Too big!"),
            Ordering::Equal => {
                println!("You win!");
                break;
            }
        }

    }

}
