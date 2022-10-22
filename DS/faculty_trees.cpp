/*********
 * Binary Search Tree
 * for CS2233
 * Abhay Shankar K
 * cs21btech11001
*/

#include <iostream>
#include <limits>

using namespace std;

class node{
    public:
    string name;
    int age;
    char gender;// M or F
    string dept;

    node * left;
    node * right;
    // node(int val){n = val; left = nullptr; right = nullptr;}

    void clone(node * current)
    {
        name = current->name;
        age = current->age;
        gender = current->gender;
        dept = current->dept;
    }
    void show()
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
        // cin >> age;
        while(!(cin >> age) || age < 0){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Age must be a positive integer." << endl;
        }
        
        // char g;
        cout << "Enter gender (M or F)." << endl;
        // cin >> g; 
        while(!(cin >> gender) || (gender != 'M' && gender != 'F')){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entered gender " << gender << " is not supported. Please enter 'M' or 'F'." << endl;
        }
        // gender = g;
        cout << "Enter department." << endl;
        cin >> dept;
        cout << endl;

        cout << "Your data has been processed." << endl;
    }

};

class Tree{
    public:
    node * root;

    void insert();
    void remove(string name);
    node * search(string name);

    void inorder(node * root);
    void preorder(node * root);
    void postorder(node * root);

    ~Tree();
    void start();
};

//usual insert
void Tree::insert()
{
    node * in = new node();
    if(!root){
        root = in;
        cout << "Tree has been initialized." << endl;
        return;
    }

    node * current = root;

    while(current){
        if(in->name < current->name){
            if(!current->left) {
                current->left = in;
                // current->left->parent = current;
                return;
            }
            else{
                current = current->left;
            }
        }
        else if(in->name > current->name){
            if(!current->right) {
                current->right = in;
                // current->right->parent = current;
                return;
            }
            else{
                current = current->right;
            }
        }
        else {cout << "Duplicate value not inserted." << endl; return;}
    }
}

//dfs
node * Tree::search(string name)
{
    node * current = root;
    while(current != nullptr){
        if(current->name == name) return current;

        else if(current->name > name) current = current->left;

        else current = current->right;
    }

    return current;
}

//the big one
void Tree::remove(string name)
{
    if(root == nullptr){
        cout << "Tree is empty." << endl;
        return;
    }

    node * req = root, * prev;

    //searching
    while(req != nullptr){
        if(req->name == name) break;
        else if(req->name > name) {prev = req; req = req->left;}
        else {prev = req; req = req->right;}
    }

    //null check
    if(req == nullptr){
        cout << "No such entry." << endl;
        return;
    }
    if(req == root) cout << "Root reassigned." << endl;

    node * t = req->right;

    //left subtree
    if(t == nullptr){
        //root
        if(req == root){
            root = root->left;
            delete req;

            return;
        }

        //leaf
        if(!req->left){
            if(prev->left == req) prev->left = nullptr;
            if(prev->right == req) prev->right = nullptr;
            delete req;
            return;
        }

        //general
        if(req == prev->left) prev->left = req->left;
        else prev->right = req->left;
        delete req;
        return;
    }

    //right node is successor
    if(t->left == nullptr){
        req->clone(t);
        req->right = t->right;
        delete t;
        return;
    }

    //general

    //successor
    while(t->left->left != nullptr) t = t->left;
    node * u = t->left;
    t->left = u->right;

    //key step
    req->clone(u);
    delete u;
}

void Tree::inorder(node * current)
{
    if(!current) return;
    inorder(current->left);
    current->show();
    inorder(current->right);
}

void Tree::preorder(node * current)
{
    if(!current) return;

    current->show();
    preorder(current->left);
    preorder(current->right);
}

void Tree::postorder(node * current)
{
    if(!current) return;
    postorder(current->left);
    postorder(current->right);
    current->show();
}

//UI
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
        else if(cmd == "in"){
            std::cout << std::endl;
            inorder(root);
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

Tree::~Tree()
{
    if(root->left != nullptr){
        Tree * left = new Tree();
        left->root = root->left;
        delete left;
    }

    if(root->right != nullptr){
        Tree * right = new Tree();
        right->root = root->right;
        delete right;
    }

    // cout << root->n << endl;
    delete root;
}

int main()
{
    Tree * t = new Tree();
    t->start();

    delete t;
    return 0;
}