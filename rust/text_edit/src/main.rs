pub struct TextEditor {
    text: String // Private member variable
}

impl TextEditor {
    pub fn new() -> TextEditor {
        TextEditor{ text: String::new() }
    }

    // modify text
    pub fn add_char(&mut self, ch : char) {
        self.text.push(ch);
    }
}

fn main() {
    let mut editor = TextEditor::new();

    editor.add_char('a');
    editor.add_char('b');
    editor.add_char('c');
}
