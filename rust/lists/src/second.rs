// in second.rs

// pub says we want people outside this module to be able to use List
pub struct List<T> {
    head: Link<T>,
}

// type aliases!
type Link<T> = Option<Box<Node<T>>>; // option lets you use "some" and "none"
// since option either IS or ISNT.

struct Node<T> {
    elem: T,
    next : Link<T>,
}

pub struct IntoIter<T>(List<T>);

pub struct Iter<'a, T: 'a> {
    next: Option<&'a Node<T>>,
}

impl<T> List<T> {
    pub fn new() -> Self {
        List { head: None }
    }

    pub fn into_iter(self) -> IntoIter<T> {
        IntoIter(self)
    }

    pub fn iter<'a>(&'a self) -> Iter<'a, T> {
        Iter { next: self.head.as_ref().map(|node| &**node) }
    }

    pub fn push(&mut self, elem: T) {
        let new_node = Box::new(Node {
            elem: elem,
            next: self.head.take(), // take is the
            // mem::replace(&mut self.head, T::Empty); , for types that are described as option
        });

        self.head = Some(new_node);
    }

    pub fn pop(&mut self) -> Option<T> {
        self.head.take().map(|node| {
            let node = *node;
            self.head = node.next;
            node.elem // uh this is a map
        })
    }

    // if you have a matching for an option that has anything
    // like the following lines
    // then you can replcae it with a map like above

//    pub fn pop(&mut self) -> Option<i32> {
//        match mem::replace(&mut self.head, Link::Empty) { // this is doing the removal part of pop for u
//            None => None, // when we're matching we match the destination
//            Some(node) => {
//                let node = *node; // this grabs the value of the node out of the box
//                self.head = node.next;
//                Some(node.elem)
//            }
//        }
//    }

    // immutable peek
    pub fn peek(&self) -> Option<&T> {
        self.head.as_ref().map(|node| {
            &node.elem // map takes the value out as map covers for mem::replace
            // which takes the value out, need to get by ref
        })
    }

    pub fn peek_mut(&mut self) -> Option<&mut T> {
        self.head.as_mut().map(|node| {
            &mut node.elem // this one is fine because its in a function
        })
    }

}

impl<T> Drop for List<T> {
    fn drop(&mut self) {
        let mut cur_link = self.head.take();
        while let Some(mut boxed_node) = cur_link {
            cur_link = boxed_node.next.take();
        }
    }
}

impl<T> Iterator for IntoIter<T> { // this is iterating and taking the values out
    type Item = T;
    fn next(&mut self) -> Option<Self::Item> {
        self.0.pop()
    }
}

impl<'a, T> Iterator for Iter<'a, T> { // this one will just be iterating without taking values out
    type Item = &'a T; // but this pointer might live forever?? since we're returning a bunch of refs

fn next(&mut self) -> Option<Self::Item> {
        self.next.map(|node| {
            self.next = node.next.as_ref().map(|node| &**node);
            &node.elem // yields a reference to the gotten node
            // because we're not consuming
            // the map is the usual mem::replace trick
            // under a bunch of abstractions
        })
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

    #[test]
    fn peek() {
        let mut list = List::new();
        assert_eq!(list.peek(), None);
        assert_eq!(list.peek_mut(), None);
        list.push(1);
        list.push(2);
        list.push(3);

        assert_eq!(list.peek(), Some(&3)); // peek returns a ref
        assert_eq!(list.peek_mut(), Some(&mut 3)); // peek mut returns mut ref
        assert_eq!(list.pop(), Some(3)); // pop actually returns the value
    }

    #[test]
    fn into_iter() {
        let mut list = List::new();
        list.push(1);
        list.push(2);
        list.push(3);

        let mut iter = list.into_iter();
        assert_eq!(iter.next(), Some(3));
        assert_eq!(iter.next(), Some(2));
        assert_eq!(iter.next(), Some(1));
    }

    #[test]
    fn iter() {
        let mut list = List::new();
        list.push(1);
        list.push(2);
        list.push(3);

        let mut iter = list.iter();
        assert_eq!(iter.next(), Some(&3));
        assert_eq!(iter.next(), Some(&2));
        assert_eq!(iter.next(), Some(&1));
    }
}

