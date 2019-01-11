extern crate base64;
extern crate hex;

// recommendation is to prefer usage of &str over String
// unless String is necessary
// a function can return a reference only when it returns a value that is
// or is contained in one of the input parameters, and when the inputs are
// either shared or mutable.
// when that's not possible, use a value like String instead

// original no return
//pub fn hex_to_byte(s : &String) {
//
//    let bytes = &hex::decode(s);
//    let ret = match bytes {
//        Ok(v) => base64::encode(v),
//        Err(e) => panic!("Invalid UTF-8 sequence: {}", e)
//    };
//
//    println!("The string is: {}", ret);
//
//}

// utility to avoid typing the match pattern
// there's probably a macro that lets me do this
pub fn hex_to_byte(s: String) -> Vec<u8> {

    match hex::decode(s) {
        Ok(v) => v,
        Err(e) => panic!("Invalid UTF-8 sequence: {}")
    }

}

// for question 1
pub fn hex_to_base64(s : String) -> String {

    match &hex::decode(s) {
        Ok(v) => base64::encode(v),
        Err(e) => panic!("Invalid UTF-8 sequence: {}", e)
    }

}

// for question 2, takes two byte vectors and xors them together
// into the first one
pub fn xor(mut s : Vec<u8>, t : Vec<u8>) -> Vec<u8> {

    for (a, b) in s.iter_mut().zip(t.iter()) {
        *a ^= *b;
    };

    s // function alters the first vector and returns it

}

// detect a single key xor cipher
pub fn detect_single_key_xor() {

}

#[cfg(test)]
mod tests {

    use super::*;

    #[test]
    fn test_hex_to_byte() {
        let hex_s = String::from("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d");
        assert_eq!(hex_to_base64(hex_s), String::from("SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t"));
    }

    #[test]
    fn test_xor() {
        let str_1 = String::from("1c0111001f010100061a024b53535009181c");
        let str_2 = String::from("686974207468652062756c6c277320657965");

        // work with raw bytes or something like that
        let str_1_vec = hex_to_byte(str_1);
        let str_2_vec = hex_to_byte(str_2);

        assert_eq!(hex::encode(xor(str_1_vec, str_2_vec)), String::from("746865206b696420646f6e277420706c6179"));
    }

}
