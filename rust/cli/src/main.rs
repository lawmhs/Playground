extern crate clap;

use clap::{App, Arg, SubCommand};
use std::{process};

fn main() {

    let matches = App::new("MyApp")
        .version("0.1")
        .author("ME!")
        .about("Playground for testing out command line applications")
        .subcommand(SubCommand::with_name("quit")
            .version("1.0") // lol quitting has a version
            .about("quits program")
        )
        .subcommand(SubCommand::with_name("palindrome")
            .version("1.0")
            .about("checks whether input string is a palindrome")
            .arg_from_usage("-s, --string, 'the string to be checked'"))
        .get_matches();

    match matches.subcommand_name() {
        Some("quit") => {
            println!("Goodbye!");
            process::exit(0);
        },
        Some("palindrome") => {
            if palindrome(matches.value_of("input")
                .expect("there should be an argument")) {
                println!("The string {} is a palindrome!", matches.value_of("input").unwrap());
            }
            else {
                println!("The string {} is not a palindrome!", matches.value_of("input").unwrap());
            }
        }
        None => println!("No subcommand was used!"),
        _ => println!("Invalid subcommand was used!")

    }

}

fn palindrome(s: &str) -> bool {
    true
}
