// in first.rs
use std::mem;

// pub says we want people outside this module to be able to use List
pub struct List {
    head: Link,
}

enum Link {
    Empty,
    More(Box<Node>),
}

struct Node {
    elem: i32,
    next : Link,
}

// to associate code with a type, use impl blocks:
impl List {
    pub fn new() -> Self { // self is alias for type wrote next to impl
        List { head: Link::Empty } // instantiates an empty linked list
    }

    pub fn push(&mut self, elem: i32) {
        let new_node = Box::new(Node {
            elem: elem,
            next: mem::replace(&mut self.head, Link::Empty), // lets you copy things when you dont own it basically
        }); // we replace self.head with empty because we don't own self.head

        self.head = Link::More(new_node); // self's head is a pointer
    }

    pub fn pop(&mut self) -> Option<i32> {
        // Option<T> is an enum that represents a value that may exist, could be either
        // Some<T> or None. Could make own enum for this, but option is ubiquitous
        // Option is generic over T
        match mem::replace(&mut self.head, Link::Empty) { // this is doing the removal part of pop for us
            // by setting that part of the list to be "Empty"
            // then inside the match we set the new head, with our reference to self
            // We then create a new node to grab the next value out
            // and then set head's next to be node's next
            // then we return the node that we grabbed!
            // Box owns its own contents
            Link::Empty => None, // when we're matching we match the destination
            Link::More(boxed_node) => {
                let node = *boxed_node; // this grabs the value of the node out of the box
                self.head = node.next;
                Some(node.elem)
            }
            // also pattern matches move the value they match on
            // so if you don't own the thing you're trying to match
            // the compiler will angery
            // so you can use the memory replace trick
            // because you know that either the element is empty (so setting it to be empty changes nothing)
            // or you're going to be replacing it with the new value so setting it to be empty
            // isn't going to be permanent anyway

            // also the only thing you can't do with &mut is move the value out with no replacement
            // so that's why we need the move replace trick
        }
    }
}

impl Drop for List {
    fn drop(&mut self) {
        let mut cur_link = mem::replace(&mut self.head, Link::Empty);
        while let Link::More(mut boxed_node) = cur_link {
            cur_link = mem::replace(&mut boxed_node.next, Link::Empty);
            // boxed_node goes out of scope and gets dropped here;
            // but its node's next field that has been set to Link::Empty
            // so there' no unbounded recursion?
        }
    }
}

#[cfg(test)]
mod test {
    use super::List;
    #[test]
    fn basics() {
        // TODO
        let mut list = List::new();

        // check empty list behaves right
        assert_eq!(list.pop(), None);

        // Populate list
        list.push(1);
        list.push(2);
        list.push(3);

        // Check normal removal
        assert_eq!(list.pop(), Some(3));
        assert_eq!(list.pop(), Some(2));

        // push some more just to make sure nothing's corrupted
        list.push(4);
        list.push(5);

        // check normal removal
        assert_eq!(list.pop(), Some(5));
        assert_eq!(list.pop(), Some(4));

        // Check exhaustion
        assert_eq!(list.pop(), Some(1));
        assert_eq!(list.pop(), None);

    }
}


// in rust the last expression is implicitly returned. You can still use return to return early
// like other C like languages though


// footnotes

// box is the simplest kind of heap allocation in rust
// so, dynamic/runtime allocation
// recursive structures must be boxed, otherwise he size of a list depends
// on how many elements are in a list, so we won't know how much memory to allocate
// since boxes have a defined size, we know how much memory we'll need

// enums in rust has an integer to store which variant of an enum it represents
// it also needs enough space to store the largest of the enums
// even though empty is just a single bit of information, it neeeds the space for
// a pointer and an element, because it might become an element at any time

// null pointer optimization: if you have two variants in an enum
// you don't need the tag to tell you if its the null variant or not
// as the tags will be done away with and a non-zero element will be used
// to represent the non null element
// e.g. tag: 00 data: 0000
// tag: 01 data: XXXX
// with null pointer optimization, we lose the tag and just use the data
// ergo 0000 and XXXX

// you would rather have a null next rather than point to a null element
// as you would be wasting memory

// null pointer optimized form: null and not-null cases only
// element unformly allocated, ergo, all elements of the enum look the same
// in this case, look like Node

// a struct with a single field requires no overhead

