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


pub fn hex_to_byte(s : String) -> String {

    match &hex::decode(s) {
        Ok(v) => base64::encode(v),
        Err(e) => panic!("Invalid UTF-8 sequence: {}", e)
    }

}

// xors the two vectors together
pub fn xor(mut s : Vec<u8>, t : Vec<u8>) -> Vec<u8> {

    for (a, b) in s.iter_mut().zip(t.iter()) {
        *a ^= *b;
    };

    s

}

#[cfg(test)]
mod tests {

    #[test]
    fn test_hex_to_byte() {

    }

    #[test]
    fn test_xor() {

    }

}
