#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <tuple>
#include <iterator>
using namespace std;

int n = 5;

class node{
    public:
    vector<int> a;
    vector<node *>c;
    node * p;

    bool isLeaf;

    void add_key(int k);
    tuple<node *, node *, int> split();
    void promote();
    node * search(int k, int q);

    void print();
};

void node::print()
{
    for(auto i : a)
        cout << i << ", ";
    cout << endl;
}

void node::add_key(const int k)
{
    if(!isLeaf) return;
    if(c.size() == 0) c.push_back(nullptr);
    c.push_back(nullptr);

    for(auto i = a.begin(); i != a.end(); i++){
        if(*i > k) {a.insert(i, k); return;}
        // cout << "1" << endl;
    }
    a.push_back(k);
}

node * node::search(const int k, int q)
{
    q = 0;
    for(auto i = a.begin(); i != a.end(); i++){
        q++;
        if(*i == k) return this;
        if(*i > k) return c[distance(a.begin(), i)];
    }

    return *(c.end());
}

tuple<node *, node *, int> node::split()
{
    //new nodes
    node * l = new node();
    node * r = new node();
    //promoted element
    int k = a[n/2];

    //splitting topmost node, new node necessary.
    if(this->p == nullptr) {
        this->p = new node(); 
        this->p->isLeaf = false; 
        // cout << "New p." << k << endl;
        // p->a.push_back(k);
    }
    //actual splitting
    
    int i = 0;
    for(; i < n / 2; i++){
        l->a.push_back(a[i]);
        l->c.push_back(c[i]);
        if(c[i])c[i]->p = l;
    }

    l->c.push_back(c[i]);
    if(c[i]) c[i]->p = l;
    i++;

    for(; i < n; i++){
        r->a.push_back(a[i]);
        r->c.push_back(c[i]);
        if(c[i]) c[i]->p = r;
    }
    r->c.push_back(c[i]);
    if(c[i]) c[i]->p = r;
    
    // for(auto i : l->a) cout << i << "|";
    // for(auto i : r->a) cout << i << "|";

    
    //setting parents. this will be deleted.
    r->p = l->p = p;
    
    // cout << "isLeaf: " << isLeaf << endl;
    l->isLeaf = r->isLeaf = isLeaf;

    return make_tuple(l, r, k);
}

void node::promote()
{
    //creates 2 new nodes
    auto t = split();
    node * l = get<0>(t);
    node * r = get<1>(t);
    int k = get<2>(t);

    // cout << "****" << endl;
    // cout << k << endl;
    // cout << "*" << endl;
    // for(auto j : l->a) cout << j << endl;
    // for(auto j : r->a) cout << j << endl;
    // cout << "****" << endl;

    //add children
    if(p->c.size() < 1){ 
        // cout << "If." << k << endl;
        // for(auto i : p->a) cout << i << endl;
        p->c.push_back(l);
        p->c.push_back(r);
    }
    else {
        // cout << "Else." << k << endl;
        // for(auto j : p->c) j->print();
        for(auto i = p->c.begin(); i != p->c.end(); i++)
            if(*i == this){
                *i = r;
                p->c.insert(i, l);
                break;
            }
        // for(auto j : p->c) j->print();
    }

    r->p = l->p = p;
    l->isLeaf = r->isLeaf = isLeaf;

    //add key
    if(!p->a.size()) p->a.push_back(k);
    else{ 
        auto i = p->a.begin();
        for(; i != p->a.end(); i++)
            if(*i > k){
                p->a.insert(i, k);
    // cout << "!!!!!" << k << endl;
                break;
            }  

        if(i == p->a.end()) {
            p->a.push_back(k); 
        // cout << "?????" << k << endl;
        }
    }
    // for(auto i : p->a) cout << i << endl;
}


class Tree{
    public:
    node * root;

    void add_key(int k);
    pair<node *, int> search(int k);
    void print(node * q, int d, int e);
    void inorder(node * r);
};  

pair<node *, int> Tree::search(const int k)
{
    if(!root) {cout << "Empty" << endl; return {nullptr, -1};}
    node * current = root;
    int q;
    while(current){
        node * next = current->search(k, q);
        if(next == current){
            return make_pair(current, q);
        }

        current = next;
    }

    cout << "Not there." << endl;
    return {nullptr, -1};
}

void Tree::add_key(const int k){
    node * current = root; //iterative pointer

    //descent
    while(!current->isLeaf){
        int i = 0;
        //n-ary decision
        for(i = 0; i != current->a.size(); i++)
            if(current->a[i] > k)
                break;
        current = current->c[i];
    }

    //node-internal addition.
    current->add_key(k);

    //unzips btree to preserve upper bound on children
    while(current->c.size() > n){

        //extends tree
        current->promote();

        node * par = current->p;
        if(current == root) root = current->p;
        // cout << "Start." << endl;

        // for(auto i : par->c)
        //     if(i == current) {cout << "Oops." << endl; break;}

        //contents of current have been copied into separate nodes and placed in the tree.
        delete current;
        //ascends
        current = par;

        // cout << "----" << endl;
        // current->print();
        // cout << "-" << endl;
        // for(auto i : current->c){
        //     i->print();
        // }
        // cout << "----" << endl;
        // cout << current->c.size() << endl;
        // cout << "End." << endl;
    }

    // cout << "1" << endl;
    return;
    cout << "NOP" << endl;
}

void Tree::print(node * first, int d, int e)
{
    // queue<node *> q;
    // q.push(root);

        
        int k = d;
        while(d--)
            cout <<  "    ";

        cout << e << ": ";
        d = k + 1;
        first->print();
        // cout << "1\n";

        if(!first->isLeaf) 
            for(int i = 0; i < first->c.size(); i++){
                // cout << "." << endl; 
                print(first->c[i], d, i);
            }
        else return;
}

void Tree::inorder(node * r){
    if(r->isLeaf){
        r->print();
        return;
    }
    inorder(r->c[0]);
    for(int i = 0; i < r->a.size(); i++){
        cout << r->a[i] << endl;
        inorder(r->c[i + 1]);
    }
}

int main(){
    Tree * t = new Tree(); //tree init
    t->root = new node(); //root init
    t->root->isLeaf = true; //initial condition

    for(int i = 1; i < 210; i++){
        t->add_key(i); //add key
        // cout << ".";
    }

    // cout << "Added." << endl;


    //checks for parent pointer consistenccy
    // for(auto i : t->root->c) {
        // i->print();
        // cout << (i->p == t->root) << i->isLeaf << endl;
        // for(auto j : i->c)
        //     cout << (j->p == i) << j->isLeaf << endl;
            // j->print();
    // }

    //prints tree horizontally
    t->print(t->root, 0, 0);

    // t->add_key(50);
    // t->add_key(55);
    // t->add_key(54);
    // t->add_key(53);
    // t->print(t->root, 0, 0);
    // cout << "Up." << endl;
    // t->add_key(52);
    // t->add_key(51);

    // t->print(t->root, 0, 0);
    // t->inorder(t->root);
    // cout << t->root->isLeaf << endl;
}