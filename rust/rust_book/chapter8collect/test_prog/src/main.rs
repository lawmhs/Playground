fn main() {
    let mut s1 = String::from("foo");
    println!("Our string is: {}", s1);

    s1.push_str(" bar"); // push_str to push slices
    println!("Now our string is: {}", s1);

    s1.push('b');
    println!("Now our string is: {}", s1);

    let s2 = String::from("bar");
    let s3 = s1 + &s2; // in this case s3 will take the value of s1 (as s1 is a move, not a copy)
    // but since it doesn't take ownership of s2, we need a reference here

    println!("s3 is actually this: {}", s3);
    // println!("we can't print this one : {}", s1); // <- use after free lol

    // don't use multiple + because the references get unwieldly
    // use format:
    let s1 = String::from("tic");
    let s2 = String::from("tac");
    let s3 = String::from("toe");

    let s = format!("{}-{}-{}", s1, s2, s3); // <- works like println!() but it assigns the
    // value to s
    println!("Let's play {}", s);

    // rust strings are UTF8 vectors
    // length of the string is the number of bytes it takes to encode the string
    // so for english it behaves as you would think

    // but to avoid confusion, rust doesn't let you index into a string to get a character
    // as you would have no idea how many valid characters there relaly are
    // you can get a slice though
    // slices should be used with caution
    // also don't slice with 0..1, because it'll be like trying to access the first index all over again.

    // iterating over strings:

    for c in s.chars() {
        println!("{}", c);
    } // do it by char

    // or do it by byte value (luckily english is one byte per char, unlike other langs)
    for n in s.bytes() {
        println!("{}", n);
    }

    // two ways to do hashmaps

    use std::collections::HashMap;

    let mut scores = HashMap::new();

    scores.insert(String::from("Blue"), 10);
    scores.insert(String::from("Yellow"), 50);

    // other way

    let teams = vec![String::from("Blue"), String::from("yellow")];
    let score = vec![10,50];

    let scores : HashMap<_, _> = teams.iter().zip(score.iter()).collect(); // <- something is happening here lol
    // so its iterating over all the values in teams and zipping them together with the scores in score
    // and then collecting them into the hashmap lol, this way you don't have to set the hashamp to be alterable.

    // for non copy traits, ownership is moved to the hashmap.
    // if we use references in the hashmap instead, the values won't be moved into the map.
    // the values that the references point to must be valid at least as long as the hashmap.

    // hashmap returns from .get() are options

    // now do some exercises :

    // first exercise : given a list of integers, use a vector and return the mean, median and mode of the list

    let mut list =  vec![1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    let mut sum = 0;
    let mut count = 0;

    let mut freq_table = HashMap::new();

    for item in list {
        sum += item;
        count += 1;
        let entry =  freq_table.entry(item).or_insert(0);
        *entry += 1;
        println!("The sum currently is : {} ", sum);
    }

    let mean = sum / count;
    println!("The mean of the list is : {}", mean);

    // the median will also be 5 but you'd have to sort the list first

    // theres some cool stuff to find the mode with matching patterns and whatever ugh need to study that
    // a little bit more



}
