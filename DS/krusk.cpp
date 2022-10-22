#include <iostream>
#include <vector>

using namespace std;

struct Edge{
    int v1, v2, w;

    void operator = (const Edge e){
        v1 = e.v1;
        v2 = e.v2;
        w = e.w;
    }

    int operator << (const vector<int> v)
    {
        int out = 0;
        for(auto i : v){
            if(v1 == i) out++;
            if(v2 == i) out += 2;
            if(out == 3) break; 
        }

        out = 3 - out;
        return out;
    }
};

#define parent(i) ((i - 1) / 2)
#define left(i) (2*i + 1)
#define right(i) (2*i + 2)

void siftdown(vector<Edge> &a, int start, int end)
{
    int r = start;
    int lc = left(r);
    while((lc = left(r)) <= end){
        int s = r;
        if(a[s].w < a[lc].w) s = lc;
        if(lc + 1 <= end && a[lc + 1].w > a[s].w) s = lc + 1;
        if(s == r) return;
        else{
            Edge t = a[r];
            a[r] = a[s];
            a[s] = t;
            r = s;
        }
    }
}

void heapify(vector<Edge> &a)
{
    int l = a.size();
    
    for(int s = parent(l - 1); s >= 0; s--)
        siftdown(a, s, l - 1);

}

void sort(vector<Edge> &a)
{
    // for(auto i : a) cout << i << " ";
    heapify(a);
    // for(auto i : a) cout << i << " ";
    int e = a.size() - 1;
    while(e > 0){
        Edge t = a[0];
        a[0] = a[e];
        a[e] = t;

        e--;
        siftdown(a, 0, e);
    }
}

vector<Edge> Krusk(vector<Edge> &a)
{
    vector<int> v;
    vector<Edge> out;
    for(auto e : a){
        int x = e << v; //0 fr none, 1 for v1, 2 for v2, 3 for both
        if(x){
            out.push_back(e);
            if(x == 1 || x == 3) v.push_back(e.v1);
            if(x == 2 || x == 3) v.push_back(e.v2); 
        }
    }

    return out;
}

int main()
{
    vector<Edge> a;
    cout << "Enter number of edges." << endl;
    int e;
    cin >> e;
    cout << "Enter : Vertex_1 Vertex_2 Weight" << endl;
    Edge x;
    while(e--){
        cin >> x.v1 >> x.v2 >> x.w;
        if(x.v1 == x.v2){
            e++;
            cout << "No self loops." << endl;
            continue;
        }
        a.push_back(x);
    }

    sort(a);

    for(auto i : Krusk(a)) cout << i.v1 << " " << i.v2 << " " << i.w << endl;
}

