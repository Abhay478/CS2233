/*********
 * Min-Heap as array
 * for CS2233
 * Abhay Shankar K
 * cs21btech11001
*/

#include <iostream>
#include <vector>
using namespace std;

class Heap
{
    vector<int> a;
    public:

    int peek();
    void extract();
    void insert(int val);

    void start();
};

//Find min
int Heap::peek()
{
    if(a.size() > 0) return a.at(0);
    else return -1;
}

//delete min
void Heap::extract()
{
    if(a.empty()){
        cout << "Empty." << endl;
        return;
    }
    int out = peek();
    int p = 0;
    a[p] = a.at(a.size() - 1);

    a.pop_back();
    // for(auto x : a)
    //     cout << x << " ";

    // cout << endl;
    int l = 1, r = 2;
    if(l >= a.size()) {cout << out << endl; return;}
    if(r >= a.size()) r = l;

    int min = a.at(l) < a.at(r) ? l : r;
    // cout << "Stuff :" << a[p] << " " << a[min] << endl;

    while(a[p] > a[min]){
        
        int temp = a.at(min);
        a[min] = a.at(p);
        a[p] = temp;

        // for(auto x : a)
        // cout << x << " ";

        // cout << endl;

        p = min;
        l = 2*p + 1;
        r = 2*p + 2;
        if(l >= a.size()) {
            // cout << "breaking." << endl; 
            break;
        }
        if(r >= a.size()) {
            // cout << "Set." << endl; 
            r = l;
        }
        min = a.at(l) < a.at(r) ? l : r;
    }
    

    cout << out << endl;
}

//usual
void Heap::insert(int val)
{
    a.push_back(val);
    // cout << val << endl;
    int i = a.size() - 1;
    int p = (i - 1) / 2;
    while(a.at(p) > a.at(i) && p >= 0){
        // cout << p << i << endl;
        int temp = a.at(p);
        a[p] = a.at(i);
        a[i] = temp;

        i = p;
        p = (p - 1) / 2;
    }
}

//UI
void Heap::start()
{
    cout << "Following are the acceptable commands:\n\n"
    "add <value> : adds value to heap.\n"
    "peek : returns min value.\n"
    "ext : deletes min value and prints it.\n"
    "destroy : deletes heap and terminates program.";

    string cmd;
    while(true){
        cout << "Enter command.\n\n>>> ";
        cin >> cmd;
        if(cmd == "add"){
            int val;
            cin >> val;
            insert(val);
        }
        else if(cmd == "peek"){
            cout << peek() << endl;
        }
        else if(cmd == "ext"){
            extract();
        }
        else if(cmd == "destroy") return;
        else cout << "Invalid command.\n" << endl;
    }
}

int main()
{
    Heap * h = new Heap();
    
/*
    // vector<int> v = { 1, 2, 3, 4, 5, 6, 7, 8, 11, 12, 13, 14, 15, 16 };
    // for(auto i : v){
    //     h->insert(16 - i);
    // }
    // // for(auto x : h->a)
    // //     cout << x << endl;
    // for (int i = 0; i < v.size() + 1; i++) {
    //     h->extract();
    // }
    // h->extract();
*/
    
    h->start();

    delete h;
    
}
