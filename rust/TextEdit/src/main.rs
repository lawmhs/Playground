pub struct TextEditor {
    test: String // Private member variable
}

impl TextEditor {
    pub fn new() -> TextEditor {
        TextEditor{ test: String::new() }
    }

    // modify text
    pub fn add_char(&mut self, ch : char) {
        self.text.push(ch);
    }
}

fn main() {
    println!("Hello, world!");
}
