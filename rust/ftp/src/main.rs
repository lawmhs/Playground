extern crate bufstream;

use std::io::prelude::*;
use std::io::{stdin, stdout, BufRead, BufReader, BufWriter};
use std::process::exit;
use std::env;
use std::net::{Ipv4Addr, SocketAddrV4, TcpStream, TcpListener};
use std::process::Command;
use bufstream::BufStream;

/// main loop of the code
fn main() -> std::io::Result<()> {

    let args: Vec<String> = env::args().collect();
    // args[1] for the first argument
    // and then whatever next

    let server_addr = &args[1];
    let port = &args[2].parse::<i32>().expect("Should be a number here");

    // create an address
    let socket_addr = SocketAddrV4::new(Ipv4Addr::new(127, 0, 0, 1), 60000);

    // then create a socket
    let mut stream = match TcpStream::connect(socket_addr) {
        Ok(s) => {
            println!("> Connected to server!");
            s
        },
        Err(e) => panic!("> Failed to connect to server")
    };

    let mut line = String::new();
    let mut buf = BufStream::new(stream);

    // whatever you see from the server
    let _ = buf.read_line(&mut line);



    loop {

        let mut input = String::new();

        print!("> ");

        let _ = stdout().flush();
        stdin().read_line(&mut input).expect("Invalid input");

        let mut input = input.split_whitespace();

        match input.next().expect("Command should not be empty") {

            "user" => {
                let user = input.next().expect("Require second argument");
                buf.get_mut().write_all(format!("{} {}\r\n", "USER", user).as_ref());
                line.clear();
                buf.read_line(&mut line);
                println!("< {}", line.trim());
            },
            "pw" => {
                let pass = input.next().expect("Require entered password");
                buf.get_mut().write_all(format!("{} {}\r\n", "PASS", pass).as_ref());
                line.clear();
                buf.read_line(&mut line);
                println!("< {}", line.trim());
            }
            "feat" => {
                buf.get_mut().write_all("FEAT\r\n".as_ref());

                // not the most elegant way to do things
                line.clear();
                buf.read_line(&mut line);
                while !line.to_lowercase().contains("end") {
                    println!("< {}", line.trim());
                    line.clear();
                    buf.read_line(&mut line);
                }
                println!("< {}", line.trim()); // messy way to do things
            }
            "quit" => {
                println!("> Goodbye!");
                buf.get_mut().write_all("QUIT\r\n".as_ref());
                exit(0);
            },
            _ => println!("Unknown command!")
        }
    }

    Ok(())

}
