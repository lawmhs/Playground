pub mod client;

pub mod network;

#[cfg(test)]
mod tests {

    use super::client; // or just call it in scope

    #[test]
    fn it_works() {
        client::connect(); // if its a sibling module, need :: in front, super moves us up one level
    }
}
