mod outermost {
    pub fn middle_function() {}

    pub fn middle_secret_function() {}

    pub mod inside {
        pub fn inner_function() {}
        pub fn secret_function() {}
    }
}

fn try_me() {
    outermost::middle_function();
    outermost::middle_secret_function(); // will have error, because middle secret not public
    outermost::inside::inner_function(); // well, module is entirely not public so i mean this will have problems too
    outermost::inside::secret_function(); // this one is also not public
}

#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        assert_eq!(2 + 2, 4);
    }
}
