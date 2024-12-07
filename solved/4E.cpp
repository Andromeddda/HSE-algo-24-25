#include <iostream>
#include <fstream>
#include <algorithm>

#include <utility>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
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
    vector<int_t> nodes;    // parents
    vector<int_t> sizes;    // component sizes
    set<int_t> components;  // ordered set of component roots
    int_t total;            // total amount of components

    DSU(int_t N);
    ~DSU() = default;

    int_t find(int_t index);
    int_t size(int_t index);
    void unite(int_t a, int_t b);

};

struct BinGraph
{
    vector<set<int>> neighbours; // ordered sets of neighbours

    BinGraph(int V) : neighbours(V) {}

    void add_edge(int x, int y) 
    {
        neighbours[x].insert(y);
        neighbours[y].insert(x);
    }
};


int main(void)
{
    // Read the graph
    int V, E;
    cin >> V >> E;

    DSU dsu(V);
    BinGraph graph(V);

    for (int i = 0; i < E; i++)
    {
        int x, y;
        cin >> x >> y;
        --x; --y;
        graph.add_edge(x, y);
    }

    // For each node v we need to decide with which nodes u (u < v)
    // we should unite it
    for (int v = 0; v < V; v++)
    {
        // For each component r < v (r is a root of component)
        // calculate the amount of edges from r to v
        unordered_map<int_t, int> component_to_edges;

        // iterate through u < v such that w(u, v) = 1
        for (int u : graph.neighbours[v])
        {
            if (u > v) // set of neighbours is ordered
                break;

            component_to_edges[dsu.find(u)] += 1;
        }

        // If the amount of nodes in r is greater than the amount of 1-edges from r to v
        // then there is at least one 0-edge from r to v.
        // In this case, unite r and v
        for (int_t component : dsu.components)
        {
            if (component >= v) // set if components is ordered
                break;

            if (dsu.size(component) > component_to_edges[component])
                dsu.unite(component, v);
        }       
    }

    cout << dsu.total - 1 << '\n';

    return 0;
}

DSU::DSU(int_t N) : nodes(N), sizes(N, 1), total(N) { 
    for (int_t i = 0; i < N; i++){
        nodes[i] = i;
        components.insert(i);
    }
}

int_t DSU::find(int_t index) {
    if (nodes[index] == index) 
        return index;

    nodes[index] = find(nodes[index]); // parent
    sizes[index] = sizes[nodes[index]]; // set size to parent's size
    return nodes[index];
}

int_t DSU::size(int_t index)
{
    int_t parent = find(index);
    return sizes[parent];
}

void DSU::unite(int_t a, int_t b) 
{
    int_t a_parent = find(a);
    int_t b_parent = find(b);

    if (a_parent == b_parent) 
        return;

    if (sizes[a_parent] < sizes[b_parent]) 
    {
        nodes[a_parent] = b_parent;
        sizes[b_parent] += sizes[a_parent];
        components.erase(a_parent);
    }
    else
    {
        nodes[b_parent] = a_parent;
        sizes[a_parent] += sizes[b_parent];
        components.erase(b_parent);
    }

    total--;
}
