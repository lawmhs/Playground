extern crate clap;

use clap::{App, Arg, SubCommand};
use std::{process};
use std::fs::{File};
use std::path::{Path};
use::std::io::prelude::*;
use std::io::{BufReader};

fn main() {

    let matches = App::new("MyApp")
        .version("0.1")
        .author("ME!")
        .about("Playground for testing out command line applications")
        .subcommand(SubCommand::with_name("grep")
            .version("0.1")
            .about("grep clone")
            .arg(Arg::with_name("expr")
                .required(true)
                .index(1)
                .help("expression to grep for"))
            .arg(Arg::with_name("file")
                .required(true)
                .index(2)
                .help("file to look into with the expr"))
        )
        .subcommand(SubCommand::with_name("palindrome")
            .version("1.0")
            .about("checks whether input string is a palindrome")
            .arg(Arg::with_name("input")
                .required(true)
                .index(1)
                .help("The string to be checked"))
            .arg_from_usage("-s, --string 'argument is the string to be checked'"))
        .subcommand(SubCommand::with_name("quit")
            .version("1.0") // lol quitting has a version
            .about("quits program"))
        .get_matches();

    // match consumes so we need to get everything out at once
    // if you only get the name you've already partially moved the data
    // so ownership has changed
    match matches.subcommand() {
        ("grep", Some(grep_matches)) => {
            let pattern = grep_matches.value_of("expr")
                .expect("There should be a pattern to be grepped for");
            let file_path = Path::new(grep_matches.value_of("file")
                .expect("There should be a file to look for"));
            grep(pattern, file_path);
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
        },
        ("quit", Some(_quit_matches)) => {
            println!("Goodbye!");
            process::exit(0);
        },
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

fn grep(pattern : &str, path : &Path) -> () {

    let file = File::open(path)
        .expect("Invalid path given");
    let file = BufReader::new(file); // so we can read line by line

    for line in file.lines() {
        let line = line.unwrap();
        if line.contains(pattern) {

            println!("{}", line);
        }
    }

}

