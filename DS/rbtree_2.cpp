/*******
 * Red-Black trees
 * for CS2233
 * Abhay Shankar K
 * cs21btech11001
*/

#include <iostream>
using namespace std;
#define RED true
#define BLACK false


class node{
    public:
    int n;
    node * l;
    node * r;
    node * p;
    bool c;

    node(int val, node * nil){
        this->n = val;
        l = r = nil;
        c = RED;
    }
};

class Tree{
    public:
    node * root;
    node * nil;
    void rrot(node * z);
    void lrot(node * z);

    void insert(int val);
    // void cleanup(node * z);

    node * search(int val);
    void inorder(node * root);
    void start();
    void depth(node * q, int d);
    Tree()
    {
        nil = new node(-1, nullptr);
        root = nullptr;
    }

};


//left-rotate
void Tree::lrot(node * z)
{
    //pretask
    node * p = z->p;
    node * c = z->l;
    node * g = p->p;
    z->p = g;
    //parity
    if(g) 
        if(p == g->r) g->r = z;
        else g->l = z; 
    else root = z;
    //pointer exchange
    p->p = z;
    p->r = c;
    c->p = p;
    z->l = p;

    //just in case
    root->c = BLACK;
}

//right-rotate
void Tree::rrot(node * z)
{
    //pretask
    node * p = z->p;
    node * c = z->r;
    node * g = p->p;
    z->p = g;
    //parity
    if(g)
        if(p == g->r) g->r = z;
        else g->l = z; 
    else root = z;
    //pointer exchange
    p->p = z;
    p->l = c;
    c->p = p;
    z->r = p;
    //just in case
    root->c = BLACK;
}

//inorder traversal
void Tree::inorder(node * root)
{
    if(root == nullptr){
        cout << "Empty." << endl;
        return;
    }
    //debugging aid

    // if(root->l == nullptr || root->r == nullptr){
    //     cout << "NULL at " << root->n << endl;
    //     return;
    // }

    //key recursive step
    if(root->l != nil) inorder(root->l);
    cout << root->n << ": " << (root->c ? "Red":"Black") << endl;
    if(root->r != nil) inorder(root->r);
}

//node insertion
void Tree::insert(int val)
{
    //pretask
    node * p = root;
    node * u, * g;
    //alloc
    node * n = new node(val, nil);
    if(root == nullptr){
        root = n;
        n->p = nullptr;
        n->c = BLACK;
        cout << "Init." << endl;
        return;
    }
    //descent
    while(true){
        if(val == p->n){
            cout << "Dupe." << endl;
            return;
        }
        if(val < p->n){
            if(p->l == nil){
                n->p = p;
                p->l = n;
                break;
            } else p = p->l;
        }
        else{
            if(p->r == nil){
                n->p = p;
                p->r = n;
                break;
            } else p = p->r;
        }
    }
    // cout << "Break" << endl;

    //restoration
    do{
        g = p->p;

        //preliminary cases
        if(p->c == BLACK) return;
        if(g == nullptr) break; 
        if(p == g->l) u = g->r;
        else u = g->l;
        if(u == nil || u->c == BLACK) break;

        //colour reassignment
        p->c = BLACK;
        u->c = BLACK;
        g->c = RED;
        //ascent
        n = g;
        p = n->p;
        

    } while(p != nullptr);

    //edge cases
    if(n == root){
        n->c = BLACK; 
        return;
    }
    if(p == root){
        p->c = BLACK;
        return;
    }

    //tree rotation
    if(u == nil || u->c == BLACK){
        //restructuring to outer grandchild
        if(p == g->l && n == p->r){
            lrot(n);
            node * t = n;
            n = p;
            p = t;
        }
        if(p == g->r && n == p->l){
            rrot(n);
            node * t = n;
            n = p;
            p = t;
        }
        
        //key rotation
        if(n == p->l) rrot(p);            
        else lrot(p);
        p->c = BLACK;
        g->c = RED;
    }
}

//node search
node * Tree::search(int val)
{
    if(!root) {
        cout << "Empty." << endl;
        return nullptr;
    }
    node * x = root;
    //descent
    while(x != nil){
        if(x->n == val) return x;
        if(x->n > val) x = x->l;
        else x = x->r;
    }
    return nullptr;

}

//UI
void Tree::start()
{
    std::cout << "Following are the acceptable commands : \n\n"
    "add <value> : adds node to tree with given value.\n"
    "is <value> : prints 'yes' if value exists, 'no' otherwise.\n"
    "in : inorder traversal.\n"
    "exit : terminates program.\n\n";

    string cmd;
    while(true){
        cout << "Enter command.\n\n>>> ";
        cin >> cmd;
        if(cmd == "add"){
            int val;
            cin >> val;
            insert(val);
            // cout << "Added." << std::endl << std::endl;
        }
        else if(cmd == "tree"){
            depth(root, 0);
            cout << endl;
        }
        else if(cmd == "in"){
            std::cout << std::endl;
            inorder(this->root);
            std::cout << std::endl;
        }
        else if(cmd == "is"){
            int val;
            cin >> val;
            std::cout << std::endl;
            node * is = search(val);
            if(is) cout << "YES";
            else cout << "NO";
            std::cout << std::endl;
        }
        /*
        else if(cmd == "pre"){
            std::cout << std::endl;
            preorder(this->root);
            std::cout << std::endl;
        }
        else if(cmd == "post"){
            std::cout << std::endl;
            postorder(this->root);
            std::cout << std::endl;
        }*/
        
        else if(cmd == "exit") return;
        else{
            std::cout << "Invalid command." << std::endl << std::endl;
        }
    }
}

//debugging aid
void Tree::depth(node * q, int d)
{
    int k = d + 1;
    while(d--)
        cout << "    ";
    
    if(q == nil){cout << "NIL" << endl; return;}
    
    cout << q->n << ": " << (q->c ? "Red":"Black") << endl;

    if(q->l == q->r) return;
    depth(q->l, k);
    depth(q->r, k);
}

int main()
{
    Tree * t = new Tree();
    t->start();

    delete t;
    return 0;
}