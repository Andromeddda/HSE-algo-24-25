#include <iostream>
#include <fstream>
#include <string>

#include <algorithm>
#include <random>

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <vector>
#include <stack>
#include <list>

#include <utility>
#include <exception>

#include <cstdlib>
#include <math.h>
#include <assert.h>

using namespace std;

struct Edge
{
    int x, y;
    Edge(int X, int Y) : x(std::min(X, Y)), y(std::max(X, Y)) { }
};

bool operator== (const Edge& lhs, const Edge& rhs) { 
    return ((lhs.x == rhs.x) && (lhs.y == rhs.y));
}

bool operator< (const Edge& lhs, const Edge& rhs) { 
    return (lhs.x < rhs.x) || ((lhs.x == rhs.x) && (lhs.y < rhs.y));
}

struct EdgeHash
{
    size_t operator() (Edge e) const {
        size_t res = 0;
        res |= e.x;
        res <<= 32;
        res |= e.y;
        return std::hash<size_t>{}(res);
    }
};

typedef unordered_map<Edge, int, EdgeHash> Edges;

struct Node
{
    int id;
    unordered_set<int> neighbours;

    Node(int n) : id(n), neighbours() {}
};

struct Graph
{
    vector<Node> nodes; // nodes id's start with 0
    Edges edges; // edges id's start with 0

    Graph(int N)
    {
        for (int i = 0; i < N; i++)
            nodes.push_back(Node(i));
    }

    void add_edge(int x, int y)
    {
        nodes[x].neighbours.insert(y);
        nodes[y].neighbours.insert(x);
        edges[Edge(x, y)] = (int)edges.size();
    }

    void delete_edge(int x, int y)
    {
        nodes[x].neighbours.erase(y);
        nodes[y].neighbours.erase(x);
    }

    int edge_id(int x, int y)
    {
        return edges[Edge(x, y)];
    }

    int get_any_neighbour(int x)
    {
        if (nodes[x].neighbours.empty())
            return -1;

        unordered_set<int>::iterator It = nodes[x].neighbours.begin();
        return *It;
    }

    int degree(int x) {
        return (int)nodes[x].neighbours.size();
    }
};

int main(void)
{

    int V, E;
    cin >> V >> E;

    Graph G(V);

    for (int i = 0; i < E; i++)
    {
        int x, y;
        cin >> x >> y;

        G.add_edge(x - 1, y - 1);
    }


    // find vertex with odd degree if it exist.
    // otherwise start with zero
    int v = 0;
    for (int i = 0; i < V; i++)
        if (G.degree(i) % 2 == 1) {
            v = i;
            break;
        }

    // find euler path
    vector<int> sequence;

    stack<int> st;
    st.push(v);
    
    while (!st.empty())
    {
        int cur = st.top();
        int next = G.get_any_neighbour(cur);

        // no edges
        if (next == -1) {
            st.pop();
            sequence.push_back(cur);
            continue;
        }

        st.push(next);
        G.delete_edge(cur, next);
    }


    for (auto i = 0LU; i < sequence.size() - 1; i++)
    {
        cout << G.edge_id(sequence[i], sequence[i+1]) + 1 << ' ';
    }

    return 0;
}