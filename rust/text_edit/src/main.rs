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

    pub fn get_text_clone(&self) -> String {
        self.text.clone() // clone allocates mnew memory, copies all chars to that area
        // lots of memory overhead
    }

    // this method doesn't work because
    // you don't own the content of self
    // so you can't just cough it up like that
    // since you're only borrowing self
//    pub fn get_text_copy(&self) -> String {
//        return self.text;
//    }

    pub fn get_text<'a>(&'a self) -> &'a String {
        return &self.text;
    }

    pub fn reset(&mut self) {
        self.text = String::new();
    }


}

// now do the car stuff:
struct Car {
    model : String
}

struct Person<'a> {
    car : Option<&'a Car> // could be empty or not
}

impl <'a> Person <'a> {
    fn new() -> Person<'a> {
        Person{ car : None }
    }

    fn buy_car(&mut self, c : &'a Car) {
        self.car = Some(c);
    }

    fn sell_car(&mut self) {
        self.car = None;
    }

    fn trade_with(&mut self, other : &mut Person<'a>) {
        let tmp = other.car;
        other.car = self.car;
        self.car = tmp;
    }

}


fn main() {

    // objects are destroyed in the reverse order that they are created
    // stack

    // for cars:
    let mut ghibli = Car{ model : "Masterati Ghibli".to_string() };
    let civic = Car{ model : "Honda Civic".to_string() };
    let mut bob = Person::new(); // must have bob after car
    let mut alice = Person::new();
    // to guarantee that car still exists when bob buys the car
    // if bob is before the car, the car would be destroyed before bob buys the car!

    bob.buy_car(&ghibli); // bob borrows ghibli
    // let g = ghibli; // this doesn't work because bob has ghibli still
    // bob.buy_car(&civic);

    // likewise even if you eliminate bob's ghibli and then run p1 below
    // it won't work because the compiler doesn't know bob has a new car!
    // (that is, that bob is borrowing a new car)
    //let p1 = &mut ghibli; // can't mut borrow this because right now bob has a mutable borrow on ghibli
    //let p2 = &ghibli; // infinite immutable borrows, or only one mutable borrow

    //bob.trade_with(&mut alice); // now alice has the mutable borrow on ghibli
    //alice.sell_car();

    // for the textEdit part
//    let mut editor = TextEditor::new();
//
//    editor.add_char('a');
//    editor.add_char('b');
//    editor.add_char('c');
//
//    let my_text = editor.get_text();
//
//    //editor.reset(); // odesn't work because we're using the reference after its freed
//
//    println!("this is my text: {}", my_text);


}
