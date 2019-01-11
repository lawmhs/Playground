extern crate hex;

mod lib;

// only operate on raw bytes... or something

fn main() {

    // question 1:
    println!("\n Concerning Question 1: \n");

    let hex_s = String::from("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d");
    println!("The hex string is : {}", hex_s);

    let ret = lib::sol::hex_to_byte(hex_s);

    println!("The base 64 version of the hex string is : {}", ret);

    // println!("{}", hex_s); //testing this, shouldn't be allowed to do this
    // because we've already moved the value in hex_s to ret.

    // question 2:
    println!("\n Conerning Question 2: \n");

    let str_1 = String::from("1c0111001f010100061a024b53535009181c");
    let str_2 = String::from("686974207468652062756c6c277320657965");

    // work with raw bytes or something like that
    let str_1_vec = match hex::decode(str_1.clone()) {
        Ok(v) => v,
        Err(e) => panic!("uh oh! {}", e)
    };
    // clone to allow copying, need to explicitly denote that you're going to be using a copied version
    // so that you can use the original values later without a use-after-move
    let str_2_vec = match hex::decode(str_2.clone()) {
        Ok(v) => v,
        Err(e) => panic!("ruh roh! {}", e)
    };

    let res = hex::encode(lib::sol::xor(str_1_vec, str_2_vec));

    println!("The xor of {} and {} is {}", str_1, str_2, res);

}
