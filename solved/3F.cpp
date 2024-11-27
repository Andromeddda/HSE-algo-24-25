#include <iostream>
#include <fstream>
#include <algorithm>

#include <utility>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <random>
#include <exception>

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

struct Edge
{
    int x, y, w;
    Edge(int X, int Y, int W) : x(X), y(Y), w(W) { }
};

bool operator< (const Edge& lhs, const Edge& rhs) {
    return lhs.w < rhs.w;
}

int main(void)
{
    int V, E;
    cin >> V >> E;

    DSU dsu(V);
    vector<Edge> edges;

    for (int i = 0; i < E; i++)
    {
        int x, y, w;
        cin >> x >> y >> w;

        edges.push_back(Edge(x - 1, y - 1, w));
    }

    stable_sort(edges.begin(), edges.end());

    int min_span = 0;
    for (Edge& e : edges)
    {
        if (dsu.find(e.x) == dsu.find(e.y))
            continue;

        dsu.unite(e.x, e.y);
        min_span += e.w;
    }

    cout << min_span;

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
