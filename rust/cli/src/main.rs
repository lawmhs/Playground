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
            .arg(Arg::with_name("input")
                .required(true)
                .index(1)
                .help("The string to be checked"))
            .arg_from_usage("-s, --string 'argument is the string to be checked'"))
        .get_matches();

    // match consumes so we need to get everything out at once
    // if you only get the name you've already partially moved the data
    // so ownership has changed
    match matches.subcommand() {
        ("quit", Some(_quit_matches)) => {
            println!("Goodbye!");
            process::exit(0);
        },
        ("palindrome", Some(palindrome_matches)) => {
            let strng = palindrome_matches.value_of("input")
                .expect("There should be an argument"); // moves it into strng
            if palindrome(strng) {
                println!("The string {} is a palindrome!", strng);
            }
            else {
                println!("The string {} is not a palindrome!", strng);
            }
        }
        ("", None) => println!("No subcommand was used!"),
        _ => println!("Invalid subcommand was used!")

    }

}

fn palindrome(s: &str) -> bool {

    s.char_indices().zip(
        s.char_indices().rev())
        .take_while(|&((first_count, _) , (last_count, _))| {first_count < last_count})
        .all(|((_, first_char), (_, last_char))| {
            first_char.to_ascii_lowercase() == last_char.to_ascii_lowercase()
        })

}
