// let's try with one mutex representing one fork

use std::sync::{Arc, Mutex};
use std::thread;

struct Philosopher {
    name: String
}

impl Philosopher {
    fn new(name : &str) -> Philosopher {
        Philosopher { name: String::from(name) }
    }

    fn greeting(&self) {
        println!("Hello! My name is {}!", self.name );
    }
}

fn main() {

    let a = Philosopher::new("Socrates");
    let b = Philosopher::new("Plato");
    let c = Philosopher::new("Aristotle");

    a.greeting();
    b.greeting();
    c.greeting();

}
