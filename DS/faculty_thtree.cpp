/*********
 * Threaded Binary Search Tree
 * for CS2233
 * Abhay Shankar K
 * cs21btech11001
*/

#include <iostream>
using namespace std;

class node{
    public:
    string name;
    int age;
    char gender;
    string dept;
    node * left;
    node * right;

    bool lth, rth;

    void left_attach(node * in);
    void right_attach(node * in);

    node * next();
    node * prev();
    void clone(node * current)
    {
        name = current->name;
        age = current->age;
        gender = current->gender;
        dept = current->dept;
    }

    inline void show()
    {
        cout << "--------" << endl;
        cout << "Name: " << name <<
        "\nAge: " << age <<
        "\nGender: " << gender <<
        "\nDepartment: " << dept << endl;
    }

    node(){
        cout << "Enter name." << endl;
        cin >> name;
        cout << "Enter age." << endl;
        cin >> age;
        char g;
        cout << "Enter gender (M or F)." << endl;
        cin >> g;
        while(g != 'M' && g != 'F'){
            cout << "Entered gender is not supported. Please enter 'M' or 'F'." << endl;
            cin >> g;
        }
        gender = g;
        cout << "Enter department." << endl;
        cin >> dept;
        cout << endl;

        lth = rth = true;

        cout << "Your data has been processed." << endl;
    }

};

//inorder successor
node * node::next()
{
    node * current = this;
    if(current->rth)
        current = current->right;
    else{
        current = current->right;
        while(!current->lth)
            current = current->left;
    }
    return current;
}

//inorder predecessor
node * node::prev()
{
    node * current = this;
    if(current->lth) current = current->left;
    else{
        current = current->left;
        while(!current->rth) current = current->right;       
    }
    return current;
}

//used to insert node to left
void node::left_attach(node * in)
{
    lth = false;
    in->left = left;
    left = in;
    in->right = this;
}

//used to insert node to right
void node::right_attach(node * in)
{
    rth = false;
    in->right = right;
    right = in;
    in->left = this;
}

class Tree{
    public:
    node * root;

    void inorder();

    void insert();
    void remove(string name);
    void remove_sub(node * req, node * p);
    node * search(string name);

    void depth(node * q, int d, int lr);

    // ~Tree(){
    //     if(!root->lth){
    //         Tree * t = new Tree();
    //         t->root = root->left;
    //         delete t;
    //     }
    //     if(!root->rth){
    //         Tree * t = new Tree();
    //         t->root = root->right;
    //         delete t;
    //     }

    //     delete root;
    // }
    ~Tree(){
        node * current = root;
        while(current->left){
            current = current->left;
        }

        node * next;
        while(current){
            next = current->next();
            delete current;
            current = next;
        }
    }
    void start();
};

//actual removal.
void Tree::remove_sub(node * req, node * p)
{
    
    cout << "Entered subroutine. Parameters : " << req->name 
    // << " " << p->n 
    << endl;
    //leaf
    if(req->lth && req->rth){
        if(req == root){
            delete req;
            root = nullptr;
            return;
        }
        if(p->left == req){ p->left = req->left; p->lth = req->lth;}
        if(p->right == req) {p->right = req->right; p->rth = req->rth;}
        cout << "Leaf." << endl;
        delete req;
        return;
    }

    //left subtree
    if(req->rth){
        cout << "thisl" << endl;
        if(req == root){
            root->prev()->right = nullptr;
            root = root->left;
            delete req;
            return;
        }
        
        if(req == p->left) p->left = req->left;
        else p->right = req->left;

        req->prev()->right = p;

        delete req;
        return;
    }

    //right subtree
    if(req->lth){
        cout << "thisr" << endl;
        if(req == root){
            cout << "root" << endl;
            root->next()->left = nullptr;
            root = root->right;
            delete req;
            return;
        }

        if(req == p->left) p->left = req->right; 
        else p->right = req->right;

        p->lth = req->lth;
        req->next()->left = p;

        delete req;
        return;
    }

}

//inserts node
void Tree::insert()
{
    node * in = new node();
    if(root == nullptr) {
        root = in;
        cout << "Tree initialised." << endl;
        return;
    }
    
    node * current = root;
    while(true)
    {
        if(in->name > current->name){
            if(current->right == nullptr){
                current->right_attach(in);
                cout << "New rightmost." << endl;
                break;;
            }
            if(current->rth) {current->right_attach(in); break;}
            current = current->right;
        }
        else if(in->name < current->name){
            if(current->left == nullptr){
                current->left_attach(in);
                cout << "New leftmost." << endl;
                break;;
            }
            if(current->lth) {current->left_attach(in); break;}
            current = current->left;
        }
        else if(in->name == current->name){
            cout << "Duplicated. Not inserted." << endl;
            break;
        }
    }

}

//traversal
void Tree::inorder()
{
    if(root == nullptr){
        cout << "Empty." << endl;
        return;
    }

    node * current = root;
    while(current->left != nullptr)
        current = current->left;

    cout << "Here goes." << endl;

    while(current != nullptr){
        current->show();
        current = current->next();
    }

}

//Doesn't actually remove, just decides how to call remove_sub
void Tree::remove(string name)
{
    if(root == nullptr){
        cout << "Tree is empty." << endl;
        return;
    }
    node * req = root, * p = nullptr;

    while(req != nullptr){
        
        if(req->name == name) break;

        else if(req->name > name) {p = req; req = req->lth ? nullptr : req->left;}

        else {p = req; req = req->rth ? nullptr : req->right;}
    }

    if(req == nullptr){
        cout << "No such entry." << endl;
        return;
    }

    cout << "Found " << req->name << ". Deleting..." << endl; //search works.
    
    if(req == root) cout << "Messing with root." << endl;

    //threaded
    if(req->lth || req->rth) remove_sub(req, p);
    
    else{
        cout << "Hamare do." << endl;
        node * current = req->next(); //current with be l-threaded.
        req->clone(current);
        cout << "Break." << endl;
        if(current->rth) p = (current != req->right)?current->right:current->left; //current is leaf. its right is its parent, except when it is its parent's right, in which case its left will be its parent.
        // else p = current->next()->right; //makes no sense.
        else{
            p = current;
            while(p->left != current){
                p = p->right;
            }
        }
    cout << "Here." << endl;
        remove_sub(current, p);
    }
}

//used in "get" and "is".
node * Tree::search(string name){
    if(root == nullptr){
        cout << "Empty." << endl;
        return nullptr;
    }
    node * current = root;
    while(true){
        if(name == current->name){
            break;
        }
        else if(name > current->name){
            if(current->rth) {current = nullptr; break;}
            current = current->right;
        }
        else if(name < current->name){
            if(current->lth)  {current = nullptr; break;}
            current = current->left;
        }
    }

    return current;
}

void Tree::depth(node * q, int d, int lr)
{
    int k = d + 1;
    while(d--)
        cout << "    ";
    cout << (lr ? "L: " : "R: ");
    cout << q->name << endl;
    if(!q->lth) depth(q->left, k, 1);
    if(!q->rth) depth(q->right, k, 0);
}

/*//UI
void Tree::start()
{
    std::cout << "Following are the acceptable commands : \n\n"
    "add <value> : adds node to tree with given value.\n"
    "del <value> : deletes node containing given value.\n"
    "is <value> : prints 'yes' if vaule exsts, 'no' otherwise."
    "in : inorder traversal.\n"
    "destroy : deletes tree and terminates program.\n\n";

    string cmd;
    while(true){
        cout << "Enter command.\n\n>>> ";
        cin >> cmd;
        if(cmd == "add"){
            insert();
            // cout << "Added." << std::endl << std::endl;
        }
        else if(cmd == "del"){
            string name;
            std::cin >> name;
            remove(name);
            // std::cout << "Deleted." << std::endl << std::endl;
        }
        else if(cmd == "in"){
            std::cout << std::endl;
            inorder();
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
        else if(cmd == "destroy") return;
        else{
            std::cout << "Invalid command." << std::endl << std::endl;
        }
    }
}
*/

void Tree::start()
{
    std::cout << "Following are the acceptable commands : \n\n"
    "add : adds node to tree.\n"
    "del <name> : deletes node containing given member.\n"
    "search <value> : prints 'yes' if vaule exists, 'no' otherwise.\n"
    "fetch <name> : prinnts data of <name>.\n"
    "in : inorder traversal.\n"
    // "pre : preorder traversal.\n"
    // "post : postorder traversal.\n"
    "exit : deletes tree and terminates program.\n\n";

    string cmd;
    while(true){
        cout << "Enter command.\n\n>>> ";
        cin >> cmd;
        if(cmd == "add"){
            insert();
            // cout << "Added." << std::endl << std::endl;
        }
        else if(cmd == "del"){
            string name;
            cin >> name;
            remove(name);
            // std::cout << "Deleted." << std::endl << std::endl;
        }
        else if(cmd == "tree"){
            if(root) depth(root, 0, 0);
            else cout << "Empty." << endl;
        }
        else if(cmd == "in"){
            std::cout << std::endl;
            inorder();
            std::cout << std::endl;
        }
        else if(cmd == "search"){
            string name;
            cin >> name;
            node * is = search(name);
            if(is) cout << "YES" << endl;
            else cout << "NO" << endl;

        }
        else if(cmd == "fetch"){
            string name;
            cin >> name;
            node * t = search(name);
            t->show();
        }
        /*
        else if(cmd == "pre"){
            std::cout << std::endl;
            preorder(root);
            std::cout << std::endl;
        }
        else if(cmd == "post"){
            std::cout << std::endl;
            postorder(root);
            std::cout << std::endl;
        }*/
        else if(cmd == "exit") return;
        else{
            std::cout << "Invalid command." << std::endl << std::endl;
        }
    }
}

int main()
{
    Tree * t = new Tree();

    t->start();
    t->depth(t->root, 0, 0);
    delete t;

}