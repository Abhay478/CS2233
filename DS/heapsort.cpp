/**********
 * Heapsort using maxheap
 * for CS2233
 * Abhay Shankar K
 * cs21btech11001
*/

#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

#define parent(i) ((i - 1) / 2)
#define left(i) (2*i + 1)
#define right(i) (2*i + 2)

void siftdown(vector<int> &a, int start, int end)
{
    int r = start;
    int lc = left(r);
    while((lc = left(r)) <= end){
        int s = r;
        if(a[s] < a[lc]) s = lc;
        if(lc + 1 <= end && a[lc + 1] > a[s]) s = lc + 1;
        if(s == r) return;
        else{
            int t = a[r];
            a[r] = a[s];
            a[s] = t;
            r = s;
        }
    }
}

void heapify(vector<int> &a)
{
    int l = a.size();
    
    for(int s = parent(l - 1); s >= 0; s--)
        siftdown(a, s, l - 1);

}

void sort(vector<int> &a)
{
    // for(auto i : a) cout << i << " ";
    heapify(a);
    // for(auto i : a) cout << i << " ";
    int e = a.size() - 1;
    while(e > 0){
        int t = a[0];
        a[0] = a[e];
        a[e] = t;

        e--;
        siftdown(a, 0, e);
    }
}

int main()
{
    vector<int> a;
    int t;
    cout << "Enter space-seperated list of integers." << endl;
    string in;
    getline(cin, in);
    stringstream ss(in);
    
    while(ss >> t) a.push_back(t);
    

    sort(a);
    for(auto i : a) cout << i << " ";
    cout << endl;
}