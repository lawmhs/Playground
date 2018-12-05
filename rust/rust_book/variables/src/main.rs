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
    let tup: (i32, f64, u8) = (500, 6.4, 1);
    let (x, y, z) = tup;
    println!("The value of y is: {}", y); // so this just grabs y out of the pattern match
    // because the pattern match makes 3 different variables

    // we can also reference the tuple elements like so:
    let five_hundred = tup.0;
    let six_point_four = tup.1;
    let one = tup.2;
    println!("The value of five_hundred is: {}", five_hundred);

    // as in java, every value of an array must have the same type:
    // useful basic array instantiation:
    let a = [1, 2, 3, 4, 5]; // implicitly rust "knows" this is an array of integers of length 5
    // but you can also denote the types explicitly like so:
    let a: [i32; 5] = [1, 2, 3, 4, 5];
    // accessing is easy
    let first = a[0];
    let second = a[1];
    // rust convention for functions is to separate words by underscores
    // camelCase for types
    another_function();

    // as you can see, functions are also statements
    // however statements don't return values
    // can't assign a let statement to another variable
    // need a return value for x to bind to
    // can't do stuff like x = y = 6;
    // 5 + 6 would be an example of an expression, which has returns

    let x = 5;
    let y = {
      let x = 3;
        x + 1
    };
    println!("tThe value of y is: {}", y);

    let five = test();
    println!("The value of the function return is : {}", five);
    let six = plus_one(five);
    println!("The value of function plus_one return is : {}", six);

    if x > 5 {
        println!("is 5 less than than {}? assuredly so", x);
    }
    else {
        println!("is 5 less than {} ? assuredly not!", x);
    }
    // if is an expression, so it can be assigned to a variable / let statement

    // functions can have return values defined

    let condition = true;
    let cantMakeVariablesDifferentTypes = if condition {
        6
    } else {
      5
    };

    /*
    loop {
        println!("again!"); // this loops until you stop manually
    }
    */

    let mut count = 5;
    while count >= 0 {
        println!("{}!", count);
        count = count - 1;
    };

    let a = [1, 2, 3, 4, 5];
    for int in a.iter() {
        println!("this array has {}", int)
    }

    for number in (1..10).rev() {
        println!("{}!", number);
    }
}

fn another_function() {
    println!("I failed my facebook phone screen :(");
}

fn test() -> i32 {
    return 5;
}

fn plus_one(x: i32) -> i32 {
    return x + 1;
    // if you don't have the return its just a statement, so there's not a return type
    // so the compiler doesn't like that!
}

// rust will not try and convert non-boolean types into a boolean
// moral of the story, you must always be explicit!



