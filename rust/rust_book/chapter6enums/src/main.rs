// in essence one can think of enums as a sort of abstraction over
// different types that are connected in some way
// ergo, colours
// or shapes
// or something like that
// then you can define a method to work on all the different types in the enum
// instead of defining one method multiple times for each of the different
// possibilities in the enum

#[derive(Debug)]
enum UsState{
    Alabama,
    Alaska,
}

enum Coin {
    Penny,
    Nickel,
    Dime,
    Quarter(UsState),
}

fn value_in_cents(coin : Coin) -> u32 {
    match coin {
        Coin::Penny => {
            println!("Lucky penny!");
            1
        },
        Coin::Nickel => 5,
        Coin::Dime => 10,
        Coin::Quarter(state) => {
            println!("State quarter from {:?}!", state);
            25
        },
    }
}

fn main() {

    let coin = Coin::Quarter(UsState::Alaska);
    println!("The value of our coin is {}", value_in_cents(coin));

    println!("Hello, world!");
}