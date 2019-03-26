struct User {
    username: String,
    email: String,
    sign_in_count: u64,
    active: bool,
}

// also can't store references in a struct without specifying lifetimes.

fn build_user(email : String, username: String) -> User {
    User {
        email,
        username,
        sign_in_count: 1,
        active: true,
    }
}

// then there are also tuple structs:

struct Color(i32, i32, i32);

fn main() {
    let mut s = String::from("hello"); // from allows creation of a string from a string literal

    s.push_str(", world!");

    println!("{}", s);

    // next section:

    let x = 5;
    let y = x; // this works because with simple primitives like this the value is copied, not moved
    // 5 is actually just straight up copied into a new memory location for y

    let s1 = String::from("hello");
    let s2 = s1; // here, s2 is now pointing to s1 on the heap
    // and actually, s1 is moved out!
    // deep copies will be required to be explicitly declared

    println!("{}, world!", s2); // can use s2, but not s1 because s1 was moved

    // in order to deep copy, use clone:
    let s1 = String::from("hello");
    let s2 = s1.clone();

    println!("s1 = {}, s2 = {}" , s1, s2);


    // integers have a "copy" trait, which allows that property to happen
    // anything with copy cannot have drop, and vice versa
    // types that are copies include i32, u32, bool, f64, char, and tuples, if they only contain copy types

    let s1 = String::from("hello");
    let len = calculate_length(&s1);
    println!("The length of '{}' is {}", s1, len);

    let mut s2 = String::from("hello");
    change(&mut s2); // s2 must be mutable to be able to borrow mutably
    println!("{}", s2);

    // also when borrowing mutably, you can only have that one borrow
    // so you can't have any additional immutable borrows before or after
    // whereas if you have only immutable borrows you can borrow as many as you like
    let mut s1 = String::from("hello world");
    let cropped = first_word(&s1);

    // s1.clear(); // built in that clears string to empty
    println!("the first word of s1's ending index is : {}", cropped); // this compiles because cropped is not connected to s1's value, we've already gotten the value out
    // but it is dangerous so we shouldn't be clearing before usage! (especially with the new changes so its not returning a number, its returning a value)

    let string = String::from("Hello World");
    let hello = &string[0..5]; // first index is inclusive, the second is exclusive (one greater than the end)
    let world = &string[6..11]; // also if you want to get the end you can do this:
    let hello = &string[..5];
    let world = &string[6..]; // going from beginning or end you don't need to include the end index
    let helloworld = &string[..];
    println!("The first word of string is {}", hello);
    println!("The seocnd word of string is {}", world);
    println!("And the entire string is {}", helloworld);

    let mut user = build_user(String::from("test email"), String::from("James"));
    println!("Our test user's name is: {}", user.username);

    user.username = String::from("Jokes"); // nice you can just set fields of structs just like that
    println!("Our test user's name is now: {}", user.username);

    let user2 = User {
        email : String::from("new User"),
        username : String::from("also new user"),
        ..user // when you want to preserve some fields
    };

    println!("Our new user's test name is now : {}", user2.username);

    let black = Color(0, 0, 0);

}

fn calculate_length(s: &String) -> usize {
    s.len()
} // at the conclusion of the function, s goes out of scope
// since s does not own the value it refers to (immutable reference), nothing happens
// to the original value of s
// since s is an immutable reference, it cannot be modified in the function

fn change(some_string: &mut String) {
    some_string.push_str(", world");
}

fn dangle() -> String {
    let s = String::from("hello");
    s
} // we return a reference to String, but s goes out of scope once we leave this function
// oh no!

// so the correct way to do this is to just return the string itself,
// then ownership is moved out of the function to whatever called it
// so nothing is deallocated, thus preventing dangling pointers / use after free!

// easy for first word, but second word is harder, as you'd have to track two indices... so there's an easier way... slices!
fn first_word(s: &String) -> &str { // &str is a string slice
    let bytes = s.as_bytes(); // converts string to array of bytes using as_bytes

    for (i, &item) in bytes.iter().enumerate() { // iter is a method that returns each element in a collection
        if item == b' ' {
            return &s[0..i];
        }
    }
    &s[..]
}