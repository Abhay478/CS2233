#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <tuple>
#include <iterator>
#include <cstdlib>
using namespace std;

//degree
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
    node * search(int k, int &q);

    void print();
    void borrow(node * &root);
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

node * node::search(const int k, int &q)
{
    q = 0;
    for(auto i = a.begin(); i != a.end(); i++){
        if(*i == k) return this;
        if(*i > k) return c[distance(a.begin(), i)];
        q++;
    }

    return c[a.size()];
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

void node::borrow(node * &root)
{
    int q = 0;
    for(auto i : p->c){
        if(i == this) break;
        q++;
    }

    if(q != p->a.size() && p->c[q + 1]->c.size() > n / 2 + n % 2){ //right sibling can donate
        a.push_back(p->a[q]); //down from parent
        p->a[q] = p->c[q + 1]->a[0]; //up from sibling
        p->c[q + 1]->a.erase(p->c[q + 1]->a.begin()); //cleanup

        c.push_back(p->c[q + 1]->c[0]); //transfer children
        p->c[q + 1]->c.erase(p->c[q + 1]->c.begin());
    }
    else if(q != 0 && p->c[q - 1]->c.size() > n / 2 + n % 2){ //left sibling can donate
        a.insert(a.begin(), p->a[q - 1]); //down from parent
        p->a[q] = *(p->c[q - 1]->a.end()); //up from sibling
        p->c[q - 1]->a.pop_back(); //cleanup

        c.insert(c.begin(), *(p->c[q - 1]->c.end())); //transfer children
        p->c[q - 1]->c.pop_back();
    }
    else{ //neither sibling can donate, so we pull one down from the parent and merge two
        if(q == p->a.size()){
            node * sib = p->c[q - 1];

            int down = p->a[q - 1];
            //pulling down
            p->a.erase(p->a.end());

            //merges sib to this
            a.insert(a.begin(), down);

            //keys
            for(auto i : sib->a)
                a.insert(a.begin(), i);

         //children
            for(auto i : sib->c)
                c.insert(c.begin(), i);

        
            p->c.erase(p->c.begin() + q - 1);
            delete sib;
        }
        
        else{
            node * sib = p->c[q + 1];

            int down = p->a[q];
            //pulling down
            p->a.erase(p->a.begin() + q);

            //merges sib to this
            a.push_back(down);

            //keys
            for(auto i : sib->a)
                a.push_back(i);

            //children
            for(auto i : sib->c)
                c.push_back(i);

        
            p->c.erase(p->c.begin() + q + 1);
            delete sib;
        }

        if(p == root){
            if(p->a.size() == 0){
                root = this;
                delete p;
                p = nullptr;
            }
        }
        else if(p->c.size() < n / 2 + n % 2){
            p->borrow(root);
        }

    }
}

class Tree{
    public:
    node * root;

    void add_key(int k);
    pair<node *, int> search(int k);
    void print(node * q, int d, int e);
    void inorder(node * r);

    void remove_key(int k);
    void start();

    Tree(){
        root = new node(); //root init
        root->isLeaf = true; //initial condition
        root->p = nullptr;
    }
};  

pair<node *, int> Tree::search(const int k)
{
    if(!root) {cout << "Empty" << endl; return {nullptr, -1};}
    node * current = root;
    int q;
    while(current){
        node * next = current->search(k, q);
        if(next == current){
            // cout << ":::" << current->a[q] << endl;
            return make_pair(current, q);
        }
        // next->print();
        current = next;
    }

    // cout << "Not there." << endl;
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
    while(current->c.size() > n || current->a.size() >= n){
        while(current->a.size() + 1 > current->c.size()){
            current->c.push_back(nullptr);
        }
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

void Tree::remove_key(int k)
{
    pair<node *, int> p = search(k);

    node * go = get<0>(p);
    int q = get<1>(p);

    if(go == root && go->isLeaf){
        go->a.erase(go->a.begin() + q);
        go->c.pop_back();
        if(go->a.size() == 0){
            cout << "Empty." << endl;
            return;
        }
    }

    if(!go){
        cout << "Not there." << endl;
        return;
    }
    // cout << q << endl;

    //successor
    if(!go->isLeaf) {
        node * next = go->c[q + 1];
        while(!next->isLeaf)
            next = next->c[0];
    
        //swap with leaf
        go->a[q] = next->a[0];
        next->a.erase(next->a.begin());
        next->c.pop_back();

        if(next->c.size() < n / 2 + n % 2)
            next->borrow(root);
    }
    else{
        go->a.erase(go->a.begin() + q);
        go->c.pop_back();
        if(go->c.size() < n/2 + n%2) go->borrow(root);
    }

}

void Tree::start()
{
    std::cout << "Following are the acceptable commands : \n\n"
    "add <value> : adds node to tree with given value.\n"
    "is <value> : prints 'yes' if value exists, 'no' otherwise.\n"
    "del <value> : deletes key.\n"
    "in : inorder traversal.\n"
    "tree: displays structure.\n"
    "exit : terminates program.\n\n";

    string cmd;
    while(true){
        cout << "Enter command.\n\n>>> ";
        cin >> cmd;
        if(cmd == "add"){
            int val;
            cin >> val;
            add_key(val);
            // cout << "Added." << std::endl << std::endl;
        }
        else if(cmd == "ch"){
            int val;
            cin >> val;
            for(int i = 0; i < val; i++){
                int k;
                cin >> k;
                add_key(k);
            }
        }
        else if(cmd == "tree"){
            print(root, 0, 0);
            cout << endl;
        }
        else if(cmd == "in"){
            std::cout << std::endl;
            inorder(this->root);
            std::cout << std::endl;
        }
        else if(cmd == "del"){
            int val;
            cin >> val;
            remove_key(val);
        }
        else if(cmd == "is"){
            int val;
            cin >> val;
            std::cout << std::endl;
            node * is = get<0>(search(val));
            if(is) cout << "YES";
            else cout << "NO";
            std::cout << std::endl;
        }
        else if(cmd == "exit") return;
        else{
            std::cout << "Invalid command." << std::endl << std::endl;
        }
    }

}


int main(){
    Tree * t = new Tree(); //tree init
    
    t->start();

    return 0;
}