#include <iostream>
#include <utility>
using namespace std;

//just an int, so no functions
class node{
    public:
    int n;
    node * p;//parent
    node * c;//highest order child
    node * r;//right sibling
    node(int k);
};

class Heap;

node::node(int k)
{
    n = k;
    p = nullptr;
    c = nullptr;
    r = nullptr;
}

//has minheap property, so should implement bubbling.
class Tree{
    public:
    node * root;
    Tree * next;
    Tree * prev;

    int order;
    //get code from minheap.
    pair<Heap *, int> extract();
    int peek();
    void merge(Tree * t);  //calling convention : argument root must have higher value than object root.
    Tree(node * r, int o);
    Tree();

    void print();
};

void Tree::print()
{
    node * current = root;
    while(current){
        node * right = current;
        while(right){
            cout << right->n << ", ";
            right = right->r;
        }
        cout << endl;
        current = current->c;
    }
}

Tree::Tree()
{
    next = nullptr;
    prev = nullptr;
}

Tree::Tree(node * r, int o)
{
    root = r;
    order = o;
    next = nullptr;
    prev = nullptr;
}

int Tree::peek()
{
    return root->n;
}

void Tree::merge(Tree * t)
{
    if(order != t->order)
        return;
    cout << "Enter tmerge." << endl;
    //root of new tree becomes highest order child of old tree 
    node * old = root->c;
    root->c = t->root;
    t->root->p = root;
    t->root->r = old;

    order++;
}

class Heap{
    public:
    Tree * head; //stored as doubly linked list, ascending order
    Tree * tail; //simplifies matters
    Tree * merge(Tree * t); //calls tree-merge. Makes sure the order is correct.
    void insert(int k); //then calls heap-merge on successive trees

    //TODO can we use vector?

    int extract(); //multiple peeks on trees, then extract on min
    void combine(Heap * h);
    void start();
    void print();
};

void Heap::print()
{
    Tree * current = head;
    while(current){
        cout << current->order << ": " << endl;
        current->print();
        cout << endl << endl;
    }
}

pair<Heap *, int> Tree::extract()
{
    int out = root->n;
    Tree * dummy = new Tree();
    Heap * h = new Heap();
    h->head = h->tail = dummy;
    node * current = root->c;

    Tree * t;
    for(int i = order - 1; i > 0; i--){
        t = new Tree(current, i);
        t->next = h->tail;
        t->prev = h->tail->prev;
        h->tail->prev = t;

        if(t->prev) t->prev->next = t;

        current = current->r;
    }

    h->head = t;
    h->tail = h->tail->prev;
    delete dummy;
    h->tail->next = nullptr;
    h->head->prev = nullptr;

    return {h, out};
}

Tree * Heap::merge(Tree * t)
{
    cout << "Enter hmerge." << endl;
    Tree * u = t->next;
    if(!u) return nullptr;
    if(t->order != u->order) return nullptr;

    //swapping trees
    if(t->root->n > u->root->n){
        node * tm = t->root;
        t->root = u->root;
        u->root = tm;
    }
    cout << "Swapped." << endl;
    //now, t.root has the lesser value.

    t->merge(u);

    cout << "MErged." << endl;

    t->next = u->next;
    if(u->next) u->next->prev = t;
    else tail = t;
    return t;
    cout << "Exit hmerge." << endl;
}

void Heap::insert(int k)
{
    node * add = new node(k);
    Tree * t = new Tree(add, 1);
    if(head == nullptr || tail == nullptr){
        head = tail = t;
        cout << "Init." << endl;
        return;
    }
    else{
        head->prev = t;
        t->next = head;
        head = t;
    }

    Tree * current = head;
    while(current && (current = merge(current)) != nullptr)
        current = current->next;
    
}

void Heap::combine(Heap * h)
{
    Tree * c1 = head;
    Tree * c2 = h->head;

    //all trees of new heap are merged with trees from original heap. Heap is restored parallely.
    while(c2 != nullptr){
        if(c1->root->n < c2->root->n){
            node * tm = c1->root;
            c1->root = c2->root;
            c2->root = tm;
        }
        
        //merges with tree from new heap
        c1->merge(c2);
        //merges with conflicting tree from current heap
        Tree * c = merge(c1);
        if(c) c1 = c;

        c1 = c1->next;
        c2 = c2->next;
    }
    
}

int Heap::extract()
{
    Tree * current = head;
    Tree * min = current;

    while(current){
        if(current->peek() < min->peek()) min = current;

        current = current->next;
    }
    // if(min->prev) min->prev->next = min->next;
    // if(min->next) min->next->prev = min->prev;

    pair<Heap *, int> p = min->extract();

    int out = get<1>(p);

    combine(get<0>(p));

    return out;
}

void Heap::start()
{
    for(int i = 1; i < 100; i++){
        insert(i);
        cout << i << endl;
    }

    // for(int i = 0; i < 10; i++){
    //     cout << extract() << endl;
    // }

    print();
}

int main()
{
    Heap * h = new Heap();

    h->start();
}

