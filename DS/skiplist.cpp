#include <iostream>
#include <vector>
#include <random>
#include <limits>
using namespace std;

class node;
class List;
class Collection;

class node{
    public:
    node * u, * d, * l, * r;
    // int n;
    string name;
    int age;
    char gender;// M or F
    string dept;

    void show()
    {
        cout << "--------------" << endl;
        cout << "Name: " << name <<
        "\nAge: " << age <<
        "\nGender: " << gender <<
        "\nDepartment: " << dept << endl;
    }

    node(int n){
        age = -1;
        name = "";
        gender = 0;
        dept = "";

        l = r = u = d = nullptr;
    }

    node(){
        // n = val;
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
        u = d = l = r = nullptr;
    }

    node(node * x){
        name = x->name;
        age = x->age;
        gender = x->gender;
        dept = x->dept;

        u = d = l = r = nullptr;
    }
};

class List{
    public:
    node * _inf;
    node * inf;

    void print_names();
    // node * search(int n);

    List(List * pr){
        _inf = new node(-1);
        inf = new node(-1);
        _inf->r = inf;
        inf->l = _inf;
        if(pr){_inf->d = pr->_inf;
        inf->d = pr->inf;}
    }
};

// node * List::search(int n){
//     node * current = _inf->r;
//     while(current != inf){
//         if(current->n == n) return current;
//         if(current->n > n) return current->l->d;
//         current = current->r;
//     }
//     return nullptr;
// }

void List::print_names()
{
    node * current = _inf;
    while(current != nullptr){
        cout << "[" << current->name << "], ";
        current = current->r;
    }

    cout << endl;
}

class Collection{
    public:
    vector<List *> sk;
    void insert();
    void remove(string n);
    node * search(string n);
    int toss();
    void print();
    void print_names();
    void start();
    Collection(){
        sk.push_back(new List(nullptr));
    }
};

void Collection::print_names(){
    for(auto i : sk){
        i->print_names();
        cout << "-----" << endl;
    }
}

void Collection::print(){
    node * current = sk.front()->_inf->r;
    while(current != sk.front()->inf){
        current->show();
        current = current->r;
    }
}

int Collection::toss()
{
    int out = 1;

    random_device rd;
    mt19937 gen(rd());
    bernoulli_distribution b(0.5);
    while(b(gen)) out++;
    return out;
}

void Collection::insert()
{
    node * below = nullptr;
    int t = toss();
    // cout << t << endl;
    if(t >= sk.size()) {
        t = sk.size(); 
        sk.push_back(new List(sk.back())); 
    // cout << "PB." << endl;
    }
    // cout << "Break." << t << endl;
    node * x = new node();
    for(auto l : sk){
        x = new node(x);
    // l = sk.front();
        t--;
        node * current = l->_inf->r;
        // if(current == l->inf) cout << "End.";
        // cout << "Dec." << endl;
        while(current->name < x->name && current != l->inf) {current = current->r;}
        if(current->name == x->name){
            cout << "Dupe." << endl;
            return;
        }

        x->l = current->l;
        x->r = current;

        current->l->r = x;
        current->l = x;
        x->d = below;
        if(below) below->u = x;

        below = x;
        if(!t) return;
    }

}

node * Collection::search(string n)
{
    node * out = sk.back()->_inf;
    // cout << (out->r == sk.back()->inf) << endl;
    // for(int i = sk.size() - 1; i >= 0; i--){
    //     out = sk[i]->search(n);
    //     if(out) break;
    // }

    while(out != sk.front()->inf){
        if(!out) return nullptr;
        // cout << out->n << endl;
        if(out->name == n) return out;
        if(out->r->name > n || out->r->age == -1){
            // cout << "Down." << (out->r->n == -1) << endl;
            out = out->d;
            continue;
        }
        out = out->r;
    }

    return nullptr;
}

void Collection::remove(string n){
    node * go = search(n);
    if(!go){
        cout << "Not there." << endl;
        return;
    }

    while(go){
        go->l->r = go->r;
        go->r->l = go->l;
        if(go->r->age == -1 && go->l->age == -1) sk.pop_back();
        cout << "Down."  << endl;
        if(go->d){go = go->d;
        delete go->u;}
        else {delete go; go = nullptr;}
    }
}

void Collection::start()
{
    std::cout << "Following are the acceptable commands : \n\n"
    "add <value> : adds node to skiplist with given value.\n"
    "is <value> : prints 'yes' if value exists, 'no' otherwise.\n"
    "del <value> : deletes key.\n"
    "dbg : prints entire skiplist.\n"
    "show : prints contents of skiplist.\n"
    "exit : terminates program.\n\n";

    string cmd;
    while(true){
        cout << "Enter command.\n\n>>> ";
        cin >> cmd;
        if(cmd == "add"){
            insert();
            // cout << "Added." << std::endl << std::endl;
        }
        else if(cmd == "ch"){
            int val;
            cin >> val;
            for(int i = 0; i < val; i++){
                // int k;
                // cin >> k;
                insert();
            }
        }
        else if(cmd == "show"){
            print();
            cout << endl;
        }
        else if(cmd == "dbg"){
            print_names();
            cout << endl;
        }
        else if(cmd == "del"){
            string val;
            cin >> val;
            remove(val);
        }
        else if(cmd == "is"){
            string val;
            cin >> val;
            node * is = search(val);
            if(is) cout << "YES";
            else cout << "NO";
            cout << endl;
        }
        else if(cmd == "exit") return;
        else{
            std::cout << "Invalid command." << std::endl << std::endl;
        }
    }

}

int main()
{
    Collection * c = new Collection();

    // for(int i = 0; i < 10; i++) cout << c->toss(100) << endl;
    c->start();

    return 0;
}
