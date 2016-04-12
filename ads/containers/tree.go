package main

type node struct {
	left, right *node
	item        string
}

type tree struct {
	count int
	root  *node
}

// func NewSplayTree() *tree {
// 	return &tree{}
// }

// func (t *tree) splay(item Item) {
// 	header := &node{}
// 	l, r := header, header
// 	var y *node
// 	root := t.root

// 	splaying := true
// 	for splaying {
// 		switch {
// 		case item.Less(root.item): // item might be on the left path
// 			if root.left == nil {
// 				splaying = false
// 				break
// 			}
// 			if item.Less(root.left.item) { // zig-zig -> rotate right
// 				y = root.left
// 				root.left = y.right
// 				y.right = root
// 				root = y
// 				if root.left == nil {
// 					splaying = false
// 					break
// 				}
// 			}
// 			r.left = root // link right
// 			r = root
// 			root = root.left
// 		case root.item.Less(item): // item might be on the right path
// 			if root.right == nil {
// 				splaying = false
// 				break
// 			}
// 			if root.right.item.Less(item) { // zig-zag -> rotage left
// 				y = root.right
// 				root.right = y.left
// 				y.left = root
// 				root = y
// 				if root.right == nil {
// 					splaying = false
// 					break
// 				}
// 			}
// 			l.right = root
// 			l = root
// 			root = root.right
// 		default: // found the item
// 			splaying = false
// 		}
// 	}
// 	l.right = root.left
// 	r.left = root.right
// 	root.left = header.right
// 	root.right = header.left
// 	t.root = root
// }

// func (st *tree) Get(key Item) Item {
// 	if st.root == nil {
// 		return nil
// 	}
// 	st.splay(key)
// 	if st.root.item.Less(key) || key.Less(st.root.item) {
// 		return nil
// 	}
// 	return st.root.item
// }

// func (t *tree) insert(item Item) {
// 	n := &node{item: item}
// 	if t.root == nil {
// 		t.root = n
// 		return
// 	}
// 	t.splay(item)
// 	switch {
// 	case item.Less(t.root.item):
// 		n.left = t.root.left
// 		n.right = t.root
// 		t.root.left = nil
// 	case t.root.item.Less(item):
// 		n.right = t.root.right
// 		n.left = t.root
// 		t.root.right = nil
// 	default:
// 		return
// 	}
// 	t.root = n
// 	t.count++
// }
