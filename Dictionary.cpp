/*
Nicolas Hak
1866459
pa8
*/

#include<string>
#include<iostream>
#include<sstream>
#include"Dictionary.h"

using namespace std;

// traversals: pre, post, and in order from
// https://www.geeksforgeeks.org/tree-traversals-inorder-preorder-and-postorder/

int const red = 0;
int const black = 1;

Dictionary::Node::Node(keyType k, valType v){
    left = nullptr;
    right = nullptr;
    parent = nullptr;
    key = k;
    val = v;
    // could have an array to hold data points, i.e.
    // double attrb[x]; (a for shorthand); x is amount of indexes needed for inventory stuffs
    // a[0] = pocket, a[1] = bank, a[2] = guns, a[3] = associates
    // a[4] = energy (max of 12), a[5] = prestige, a[6]
    color = black; // by default each inserted node is red
    // 0 for red, 1 for black
}

Dictionary::Dictionary(){
    nil = new Node("nil", -99);
    nil->color = black;
    root = nil;
    root->parent = nil;
    root->left = nil;
    root->right = nil;
    current = root;
    num_pairs = 0;
}

void Dictionary::postOrderDelete(Node* R){
    if(R == nil){
        return;
    }
    postOrderDelete(R->left);
    postOrderDelete(R->right);
    R->parent = nullptr;
    R->left = nullptr;
    R->right = nullptr;
    delete(R);
}

int Dictionary::size() const{
    return num_pairs;
}

void Dictionary::clear(){
    postOrderDelete(root);
    // since post order delete leaves nil behind
    root = nil;
    root->parent = nil;
    root->left = nil;
    root->right = nil;
    current = root;
    num_pairs = 0;
}

Dictionary::~Dictionary(){
    // postOrderDelete(root);
    clear();
    delete(nil);
}

void Dictionary::begin(){
    current = root;
    while(current->left != nil){
        current = current->left;
    }
}

void Dictionary::end(){
    current = root;
    while(current->right != nil){
        current = current->right;
    }
}

bool Dictionary::hasCurrent() const{
    if(current != nil){
        return true;
    }else{ return false; }
}

Dictionary::Node* Dictionary::findMax(Node* R){
    if(R == nil){
        throw std::logic_error("Dictionary: findMax() called on nil node");
    }
    while(R->right != nil){
        R = R->right;
    }
    return R;
}

Dictionary::Node* Dictionary::findPrev(Node* N){
    if(N->left != nil){
        return findMax(N->left);
    }
    Node* upper = N->parent;
    while(upper != nil && N == upper->left){
        N = upper;
        upper = upper->parent;
    }
    return upper;
}

void Dictionary::prev(){
    if(this->hasCurrent() == false){
        throw std::logic_error("Dictionary: prev(): current not defined");
    }
    if(findPrev(current) != nil){
        Node* next = findPrev(current);
        current = next;
    }else{
        current = nil;
    }
}

Dictionary::Node* Dictionary::findMin(Node* R){
    if(R == nil){
        throw std::logic_error("Dictionary: findMin() called on nil node");
    }
    while(R->left != nil){
        R = R->left;
    }
    return R;
}

Dictionary::Node* Dictionary::findNext(Node* N){
    if(N->right != nil){
        return findMin(N->right);
    }
    Node* upper = N->parent;
    while(upper != nil && N == upper->right){
        N = upper;
        upper = upper->parent;
    }
    return upper;
}

void Dictionary::next(){
    if(this->hasCurrent() == false){
        throw std::logic_error("Dictionary: next(): current not defined");
    }
    if(findNext(current) != nil){
        Node* next = findNext(current);
        current = next;
    }else{
        current = nil;
    }
}

keyType Dictionary::currentKey() const{
    if(current == nil){
        throw std::logic_error("Dictionary: currentKey(): current not defined");
    }
    return current->key;
}

valType& Dictionary::currentVal() const{
    if(current == nil){
        throw std::logic_error("Dictionary: currentVal(): current not defined");
    }
    return current->val;
}

void Dictionary::transplant(Node* old, Node* newer){
    if(old->parent == nil){
        root = newer;
    }else if(old == old->parent->left){
            old->parent->left = newer;
        }
    else{
        old->parent->right = newer;
    }
    newer->parent = old->parent;
}

Dictionary::Node* Dictionary::search(Node* R, keyType k) const{
    // start search from root
    Node* temp = R;
    if(temp == nil){
        return temp;
    }
    while(true){
        // if equal, return
        int comp = k.compare(temp->key);
        if(comp == 0){
            return temp;
        }
        // go left or right if not equal
        if(comp < 0){
            if(temp->left == nil){
                return nil;
            }
            temp = temp->left;
            continue;
        }
        if(comp > 0){
            if(temp->right == nil){
                return nil;
            }
            temp = temp->right;
            continue;
        }
    }
}

void Dictionary::BST_remove(keyType k){
    // pointer stuffs from lecture and from
    // https://www.geeksforgeeks.org/deletion-in-binary-search-tree/#
    if(contains(k) == false){
        std::string emsg = "remove(): K " + k + " not found";
        throw std::logic_error(emsg);
    }

    Node* toDel = search(root, k);
    //cout << "Will attempt to delete " << toDel->key << endl;
    int isCurr = 0;
    if(toDel == current){
        isCurr = 1;
    }
    // case where either has left or right child as nil
    // transplant to maintain proper pointers to any existing subtrees
    if(toDel->left == nil){
        //cout << toDel->key << " has no left\n";
        transplant(toDel, toDel->right); // switch out the pointers as necessary and delete
        delete(toDel);
    }else if(toDel->right == nil){
        //cout << toDel->key << " has no right\n";
        transplant(toDel, toDel->left);
        delete(toDel);
    }else{
        Node* placeHold = current; // act as a placeholder for original cursor pos
        current = toDel;
        // last catch where both left and right children are defined.

        this->next(); // use this to determine the successor
        Node* successor = current; 

        if(successor->parent != toDel){
            transplant(successor, successor->right);
            successor->right = toDel->right;
            successor->right->parent = successor;
        }
        transplant(toDel, successor);
        successor->left = toDel->left;
        successor->left->parent = successor;
        delete(toDel);
        current = placeHold; // set the cursor back to what it was originally
    }
    if(isCurr == 1){
        current = nil;
    }
    num_pairs--;
}

void Dictionary::deletefix(Node* x){
    while (x != root && x->color == black){
      if (x == x->parent->left){
         Node* w = x->parent->right;
         if (w->color == red){
            //cout << "case 1\n";
            w->color = black;                        // case 1
            x->parent->color = red;                   // case 1
            leftRotate(x->parent);                // case 1
            w = x->parent->right;                     // case 1
         }
         if(w->left->color == black && w->right->color == black){
            //cout << "case 2\n";
            w->color = red;                          // case 2
            x = x->parent;                           // case 2
         }
         else{ 
            if(w->right->color == black){
                //cout << "case 3\n";
               w->left->color = black;                // case 3
               w->color = red;                       // case 3
               rightRotate(w);                   // case 3
               w = x->parent->right;                  // case 3
            }
            //cout << "case 4\n";
            w->color = x->parent->color;               // case 4
            x->parent->color = black;                 // case 4
            w->right->color = black;                  // case 4
            leftRotate(x->parent);                // case 4
            x = root;                             // case 4
         }
      }
      else{ 
         Node* w = x->parent->left;
         if (w->color == red){
            //cout << "case 5\n";
            w->color = black;                        // case 5
            x->parent->color = red;                   // case 5
            rightRotate(x->parent);               // case 5
            w = x->parent->left;                      // case 5
         }
         if (w->right->color == black && w->left->color == black){
            //cout << "case 6\n";
            w->color = red;                          // case 6
            x = x->parent;                           // case 6
         }
         else{ 
            if (w->left->color == black){
                //cout << "case 7\n";
               w->right->color = black;               // case 7
               w->color = red;                       // case 7
               leftRotate(w);                    // case 7
               w = x->parent->left;                   // case 7
            }
            //cout << "case 8\n";
            w->color = x->parent->color;               // case 8
            x->parent->color = black;                 // case 8
            w->left->color = black;                   // case 8
            rightRotate(x->parent);               // case 8
            x = root;                             // case 8
         }
      }
    }
   x->color = black;
}

void Dictionary::remove(keyType k){ // RB remove;
    Node* z = search(root, k); // find the node to be deleted
    if(z == current){
        current = nil;
    }

    Node* x;
    Node* y = z;
    int org_color = z->color;

    if(z->left == nil){               // case 1  or case 2->1 (right only)
        //cout << "del case 1\n";
        x = z->right;
        
        transplant(z, z->right);
       // cout << "x.right.color " << x->right->color << endl;
        delete(z);
    }
   else if(z->right == nil){         // case 2->2 (left only)
        //cout << "del case 2\n";
        x = z->left;
        transplant(z, z->left);
        delete(z);
   }
   else{                           // case 3
   //cout << "del case 3\n";
    y = findMin(z->right);
    org_color = y->color;
    x = y->right;
    if(y->parent == z){
        //cout << "del case 3 if\n";
        x->parent = y;
    }
    else{
        //cout << "del case 3 else\n";
         transplant(y, y->right);
         y->right = z->right;
         y->right->parent = y;
      }
      transplant(z, y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
      delete(z);
   }
   num_pairs--;
   if(org_color == black){
    // cout << "fix called\n";
    // cout << "x color " << x->color << endl;
    // cout << "x.par.right.color " << x->parent->right->color << endl;
    deletefix(x);
   }
}

valType& Dictionary::getValue(keyType k) const{
    std::lock_guard<std::mutex> lock(mtx);
    Node* N = search(root, k);
    if(N == nil){
        throw std::logic_error("called getValue(k) on non-existent k");
    }
    return N->val;
}

valType* Dictionary::getArray(keyType k) const{
    std::lock_guard<std::mutex> lock(mtx);
    Node* N = search(root, k);
    if(N == nil){
        throw std::logic_error("called getArray(k) on non-existent k");
    }
    return N->inventory;
}

void Dictionary::leftRotate(Node* x){
   // set y
   Node* y = x->right; 
   
   // turn y's left subtree into x's right subtree
   x->right = y->left; 

   if(y->left != nil){     // not necessary if using sentinal nil node
      y->left->parent = x;
   }
   
   // link y's parent to x
   y->parent = x->parent;
   if(x->parent == nil){
      root = y;
   }
   else if(x == x->parent->left){
      x->parent->left = y;
   }
   else{
      x->parent->right = y;
   }
   // put x on y's left
   y->left = x; 
   x->parent = y;
}

void Dictionary::rightRotate(Node* x){
   // set y
   Node* y = x->left; 
   
   // turn y's right subtree into x's left subtree
   x->left = y->right; 
   if(y->right != nil){  // not necessary if using sentinal nil node
      y->right->parent = x;
   }
   
   // link y's parent to x
   y->parent = x->parent;
   if(x->parent == nil){
      root = y;
   }
   else if(x == x->parent->right){
      x->parent->right = y;
   }
   else{
      x->parent->left = y;
   }
   
   // put x on y's right
   y->right = x; 
   x->parent = y;
}

void Dictionary::insertFix(Node* z){
    Node* y;
    while(z->parent->color == red){
      if(z->parent == z->parent->parent->left){
         y = z->parent->parent->right;
         if(y->color == red){
            z->parent->color = black;              // case 1
            y->color = black;                     // case 1
            z->parent->parent->color = red;         // case 1
            z = z->parent->parent;                 // case 1
         }
         else{
            if(z == z->parent->right){
               z = z->parent;                     // case 2
               leftRotate(z);
            }                               // case 2
            z->parent->color = black;              // case 3
            z->parent->parent->color = red;         // case 3
            rightRotate(z->parent->parent);     // case 3
         }
      } else {
         y = z->parent->parent->left;
         if(y->color == red){
            z->parent->color = black;              // case 4
            y->color = black;                     // case 4
            z->parent->parent->color = red;         // case 4
            z = z->parent->parent;                 // case 4
         }
         else{ 
            if(z == z->parent->left){
               z = z->parent;                     // case 5
               rightRotate(z);                // case 5
            }
            z->parent->color = black;              // case 6
            z->parent->parent->color = red;         // case 6
            leftRotate(z->parent->parent);      // case 6
         }
      }
    }
   root->color = black;
}

void Dictionary::setValue(keyType k, valType v){
    // this handles case when dictionary is empty
    std::lock_guard<std::mutex> lock(mtx);
    if(root == nil){
        // cout << "Inserted first value as root." << endl;
        root = new Node(k, v); // root parent, left and right default set to nil
        root->right = nil;
        root->left = nil;
        root->parent = nil;
        num_pairs++;
        root->color = 1; // root is always black
        // current = root;
        return;
    }
    // assuming root is defined: if k < current.k
    Node* seek = root; // seek is just temp node pointer
    while(true){
        int comparison = k.compare((seek->key));
        if(comparison < 0){ // less than, go left
            if((seek->left) == nil){ // if no left child, place
                Node* inserted = new Node(k, v); // create a new node
                seek->left = inserted; // update seek's leftchild
                inserted->parent = seek; // update the new node's parent
                // when new node is made, its left/right child is nil
                inserted->left = nil;
                inserted->right = nil;
                num_pairs++;
                inserted->color = red;
                insertFix(inserted);
                return;
            }
            // in the case where seek has left child
            seek = seek->left; // go left again
            continue; // start at beginning of loop to determine whether to go left right or set
        }
        else if(comparison > 0){ // greater than, go right
            if((seek->right) == nil){ // if no right child, place
                Node* inserted = new Node(k, v); // create new node
                seek->right = inserted; // update seek's rightchild
                inserted->parent = seek; // update new node's parent
                // new node is made, its left/right child is nil
                inserted->right = nil;
                inserted->left = nil;
                num_pairs++;
                inserted->color = red;
                insertFix(inserted);
                return; 
            }
            // in the case where seek has right child
            seek = seek->right;
            continue; // start at beginning of loop to determine left right or set
        }
        else { // in the case where we encounter the same key, replace v with the new v
            seek->val = v;
            return;
        }
    }
}

// val v represents the order it was inserted in; compare operations done on key k
void Dictionary::BST_setvalue(keyType k, valType v){
    // this handles case when dictionary is empty
    if(root == nil){
        // cout << "Inserted first value as root." << endl;
        root = new Node(k, v); // root parent, left and right default set to nil
        root->right = nil;
        root->left = nil;
        root->parent = nil;
        num_pairs++;
        root->color = 1; // root is always black
        // current = root;
        return;
    }
    // assuming root is defined: if k < current.k
    Node* seek = root; // seek is just temp node pointer
    while(true){
        int comparison = k.compare((seek->key));
        if(comparison < 0){ // less than, go left
            if((seek->left) == nil){ // if no left child, place
                Node* inserted = new Node(k, v); // create a new node
                seek->left = inserted; // update seek's leftchild
                inserted->parent = seek; // update the new node's parent
                // when new node is made, its left/right child is nil
                inserted->left = nil;
                inserted->right = nil;
                num_pairs++;
                // insertFix(inserted);
                return;
            }
            // in the case where seek has left child
            seek = seek->left; // go left again
            continue; // start at beginning of loop to determine whether to go left right or set
        }
        else if(comparison > 0){ // greater than, go right
            if((seek->right) == nil){ // if no right child, place
                Node* inserted = new Node(k, v); // create new node
                seek->right = inserted; // update seek's rightchild
                inserted->parent = seek; // update new node's parent
                // new node is made, its left/right child is nil
                inserted->right = nil;
                inserted->left = nil;
                num_pairs++;
                // insertFix(inserted);
                return; 
            }
            // in the case where seek has right child
            seek = seek->right;
            continue; // start at beginning of loop to determine left right or set
        }
        else { // in the case where we encounter the same key, replace v with the new v
            seek->val = v;
            return;
        }
    }
}



bool Dictionary::contains(keyType k) const{
    if(root->val == -99){ // don't search since empty
        return false;
    }
    Node* seek = root;
    int comp = 0;
    while(true){
        if(seek == nil){
            return false;
        }
        comp = k.compare(seek->key);
        if(comp < 0){ // if k less than, go left
            seek = seek->left;
            continue;
        }
        if(comp > 0){ // if k bigger, go right
            seek = seek->right;
            continue;
        }
        if(comp == 0){ // if found, return true
            return true;
        }
    }

    return false;
}

void Dictionary::preOrderString(std::string& s, Node* R) const{
    if(R == nil){
        return;
    }
    // string v = std::to_string(R->val);
    // s = s + R->key + "\n";
    string toAdd = R->key;
    if(R->color == red){
        toAdd += " (RED)";
    }
    toAdd += "\n";
    s += toAdd;
    preOrderString(s, R->left);
    preOrderString(s, R->right);
}

string Dictionary::pre_string() const{
    string result = "";
    preOrderString(result, this->root);
    return result;
}

void Dictionary::inOrderString(std::string& s, Node* R) const{
    if(R == nil){
        return;
    }
    inOrderString(s, R->left);
    // string v = std::to_string(R->val);
    // s = s + R->key + " : " + v + "\n";
    string v =  std::to_string(R->val);
    string arr = "";
    for(int i=0; i<15; i++){
        arr += std::to_string(R->inventory[i]) + " ";
    }
    arr.pop_back(); // erase trailing space
    string toAdd = R->key + ": " + v + " " + arr + "\n";
    s+= toAdd;
    // s += R->key; s+= " : "; s+= v; s+= "\n";
    // s.append(toAdd);
    inOrderString(s, R->right);
}

string Dictionary::to_string() const{ // shows root for now
    // supposed to print in order
    string result = "";
    // string v = std::to_string(root->val);
    // result = result + root->key + " : " + v + "\n";
    inOrderString(result, this->root);
    return result;
}

void Dictionary::preOrderCopy(Node* R, Node*& N){
    // R is from original tree (root start)
    // N is root start of copy
    if(R == nil){ // if the root is NIL
        N = nil;
        N->parent = nil;
        N->right = nil;
        N->left = nil;
        return;
    }

    if(R->val == -99){
        return;
    }
    
    if(R != N){
        // cout << "R: " << R->key << endl;
        BST_setvalue(R->key, R->val);
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
}

Dictionary::Dictionary(const Dictionary& D){
    nil = new Node("nil", -99);
    nil->color = 1;
    root = nil;
    root->parent = nil;
    root->left = nil;
    root->right = nil;
    current = root;
    num_pairs = 0;
    // this chunk to create the empty obj.
    if(D.root != nil){
        preOrderCopy(D.root, this->root);
        num_pairs = D.num_pairs;
    }
}

bool Dictionary::equals(const Dictionary& D) const{
    if(this->num_pairs != D.num_pairs){
        return false;
    }
    // if(this->root->key != D.root->key || this->root->val != D.root->val){
    //     return false;
    // }
    if(this->to_string() != D.to_string()){
        return false;
    }
    return true;
}

std::ostream& operator<<( std::ostream& stream, Dictionary& D ){
    stream << D.to_string();
    return stream;
}

bool operator==( const Dictionary& A, const Dictionary& B ){
    return A.equals(B);
}

Dictionary& Dictionary::operator=( const Dictionary& D ){
    if(this != &D){
        Dictionary temp = D;
        swap(num_pairs, temp.num_pairs);
        swap(nil, temp.nil);
        swap(root, temp.root);
        swap(current, temp.current);
    }
    return *this;
}