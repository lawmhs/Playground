extern crate bufstream;

use std::io::prelude::*;
use std::io::{stdin, stdout, BufRead, BufReader, BufWriter};
use std::process::exit;
use std::env;
use std::net::{Ipv4Addr, SocketAddrV4, TcpStream, TcpListener};
use std::process::Command;
use bufstream::BufStream;
use regex::Regex;

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
            "ls" => {

                // switch to passive mode
                buf.get_mut().write_all("PASV\r\n".as_ref());
                line.clear();
                buf.read_line(&mut line); // only outputs one line

                // line will be something like:
                // 227 Entering Passive Mode (142, 103, 6, 49, 107, 180)
                // need to use regex to recognize the numbers?

                println!("{}", line.trim()); // line is printing properly

                // need to open a second connection
                let mut ip_array = [0, 0, 0, 0];
                let pasv_addr: Ipv4Addr; // this can be uninitialized because we're guaranteed to initialize this after the for
                let mut port = 0; // this must be initialized because the assignment is inside an if else, which the compiler thinks
                // we may or may not hit


                let re = Regex::new(r"(\d+),(\d+),(\d+),(\d+),(\d+),(\d+)").expect("should be a valid regex pattern");
                let caps = re.captures(line.trim()).expect("Should have a capture group from entering passive for ls command"); // should only have one capture group

                for i in 0..5 {
                    if i < 4 {
                        // the first element in the captures is not usable as a number
                        ip_array[i] = caps.get(i + 1).unwrap().as_str().parse::<u8>().expect("need valid ipv4 addr segment");
                    } else {
                        let big = caps.get(5).unwrap().as_str().parse::<u16>().expect("first part of port number invalid") * 256;
                        let small = caps.get(6).unwrap().as_str().parse::<u16>().expect("second part of port numebr invalid");
                        port = big + small;
                    }

                }

                pasv_addr = Ipv4Addr::new(ip_array[0], ip_array[1], ip_array[2], ip_array[3]);
                println!("IP Address: {}, Using Port: {}", pasv_addr, port);

                // initialize a new connection:
                let pasv_socket = SocketAddrV4::new(pasv_addr, port);



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
