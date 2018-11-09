fn main() {
    let mut x = 5;
    println!("The value of x is: {}", x);
    x = 6;
    println!("The value of x is: {}", x);

    //const MAX_POINTS: u32 = 100_000; // constants naming convention in rust is ALL CAPS UNDERSCORES
    // underscores can also be inserted in number sto readability
    // constants valid for entire runtime of program, within declared scope
    // also set hardcoded values as constants

    let x = 5;

    let x = x + 1;

    let x = x * 2; // using the let keyword is basically redefining a new variable
    // that's why the "value" changes
    // if you tried to do x = x + 1 after a let, it wouldn't happen because variables
    // are not mutable by default (that owuld be mutabling)

    println!("The value of x is: {}", x);

    // shadowing allows us to change the type of a variable
    // how like in java you have String x = " ";
    // then int x = 3;
    // ergo:
    // let spaces = " ";
    // let spaces = spaces.len();
    // versus
    // let mut spaces = " ";
    // spaces = spaces.len(); <--- this would give an error! cannot mutate string to integer!

    let guess: u32  = "42".parse().expect("Not a number!");
    println!("guess: {}", guess);
    // numbers in rust
    // specify size with i8, u8, all the way to i128, u128
    // i for signed, u for unsigned
    // rust default is i32
    // floating point types f32 and f64, by size
    // let t = true;
    // let f: bool = false; // explicit type annotation, booleans one byte in size

    // char in rust represents unicode scalar value
    let c = '😻';
    println!("lmao this character: {}", c);

    // tuples 
}

