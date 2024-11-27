#include <iostream>
#include <fstream>

#include <utility>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <random>

#include <cstdlib>
#include <math.h>
#include <assert.h>

using namespace std;

typedef int int_t;

struct DSU
{
    vector<int_t> nodes;
    int_t total;

    DSU(int_t N);
    ~DSU() = default;

    int_t find(int_t index);
    void unite(int_t a, int_t b);

};

int main(void)
{
    int N;
    cin >> N;

    unordered_map<int, int> x_to_pos;
    unordered_map<int, int> y_to_pos;

    DSU dsu(N);

    for (int i = 0; i < N; i++)
    {
        int x, y;
        cin >> x >> y;

        // if there is a snowbank on the same x coord, unite them
        if (x_to_pos.find(x) != x_to_pos.end()) {
            dsu.unite(i, x_to_pos.at(x));
        }

        if (y_to_pos.find(y) != y_to_pos.end()) {
            dsu.unite(i, y_to_pos.at(y));
        }

        x_to_pos[x] = i;
        y_to_pos[y] = i;
    }

    cout << dsu.total - 1 << '\n';



    return 0;
}

DSU::DSU(int_t N) : nodes(N), total(N) { 
    for (int_t i = 0; i < N; i++)
        nodes[i] = i;
}


int_t DSU::find(int_t index) {
    if (nodes[index] == index) 
        return index;

    nodes[index] = find(nodes[index]);
    return nodes[index];
}

void DSU::unite(int_t a, int_t b) {
    int_t a_parent = find(a);
    int_t b_parent = find(b);

    if (a_parent == b_parent) 
        return;

    if (rand() % 2 == 0)
        nodes[a_parent] = b_parent;
    else
        nodes[b_parent] = a_parent;

    total--;
}
