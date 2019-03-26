#[derive(Debug)]
struct Rectangle {
    width: u32,
    height: u32,
} // unsigned 32, can't really have negative dimensions can we?

impl Rectangle {
    fn square(dim : u32) -> Rectangle {
        Rectangle { width : dim, height : dim }
    }

    fn area(&self) -> u32 {
        self.width * self.height
    }

    fn can_hold(&self, other : &Rectangle) -> bool {
        self.width > other.width && self.height > other.height
    }
}

fn main() {

    let rec1 = Rectangle { width : 10, height : 10 };
    let rec2 = Rectangle { width : 20, height : 20 };
    let rec3 = Rectangle { width : 30, height : 30 };

    let area1 = rec1.area();
    let area2 = rec2.area();
    let area3 = rec3.area();

    let square = Rectangle::square(5); // how to call methods in impl blocks

    println!("Can rec1 hold rec2? {}", rec1.can_hold(&rec2));
    println!("Can rec2 hold rec3? {}", rec2.can_hold(&rec3));
    println!("The sum of all the areas of the rectangles is:  {}", area1 + area2 + area3);
    println!("The length of the square is : {}", square.width);
}
