// in third.rs

use std::rc::Rc;

// declaration of what a list looks like
pub struct List<T> {
    head: Link<T>,
}

// wrapping the node
type Link<T> = Option<Rc<Node<T>>>;

// declaration of a node
struct Node<T> {
    elem: T,
    next: Link<T>,
}

pub struct Iter<'a, T: 'a> {
    next : Option<&'a Node<T>>,
}

// implementation of the list
// to allow shared access to this
// the elements of this list will not be mutable
impl<T> List<T> {
    pub fn new() -> Self {
        List { head: None }
    }

    pub fn iter<'a>(&'a self) -> Iter<'a, T> {
        Iter { next :  self.head.as_ref().map(|node| &**node) }
    }

    pub fn append(&self, elem: T) -> List<T> {
        List { head : Some (Rc::new(Node {
            elem : elem,
            next : self.head.clone(),
        }))}
    }

    pub fn tail(&self) -> List<T> {
        List { head : self.head.as_ref().and_then(|node| node.next.clone() ) }
    }

    pub fn head(&self) -> Option<&T> {
        self.head.as_ref().map(|node| &node.elem )
    }
}

impl<'a, T> Iterator for Iter<'a, T> {
    type Item = &'a T;

    fn next(&mut self) -> Option<Self::Item> {
        self.next.map(|node| {
            self.next = node.next.as_ref().map(|node| &**node);
            &node.elem
        })
    }
}


impl<T> Drop for List<T> {
    fn drop(&mut self) {
        // steal the list's head
        let mut cur_list = self.head.take();
        while let Some(node) = cur_list {
            // Clone curent node's next node.
            cur_list = node.next.clone();
            // node dropped here.
            // if old node has refcount 1, then it will be dropped and freed
            // but it won't fully recurse and drop its child because we
            // hold another Rc to it.
        }
    }
}

#[cfg(test)]
mod test {
    use super::List;

    #[test]
    fn basics() {
        let list = List::new();
        assert_eq!(list.head(), None);

        let list = list.append(1).append(2).append(3);
        assert_eq!(list.head(), Some(&3));

        let list = list.tail();
        assert_eq!(list.head(), Some(&2));

        let list = list.tail();
        assert_eq!(list.head(), Some(&1));

        let list = list.tail();
        assert_eq!(list.head(), None);

        let list = list.tail();
        assert_eq!(list.head(), None);
    }

    #[test]
    fn iter() {
        let list = List::new().append(1).append(2).append(3);

        let mut iter = list.iter();
        assert_eq!(iter.next(), Some(&3));
        assert_eq!(iter.next(), Some(&2));
        assert_eq!(iter.next(), Some(&1));
    }

}