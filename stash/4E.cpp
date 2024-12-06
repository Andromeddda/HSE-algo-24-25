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

#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <assert.h>
#include <climits>
#include <cfloat>

using namespace std;

struct BinGraph
{
    vector<unordered_set<int>> neighbours;

    BinGraph(int V) : neighbours(V) {}

    void add_edge(int x, int y) 
    {
        neighbours[x].insert(y);
        neighbours[y].insert(x);
    }

    int primus();
};

int main(void) 
{
    int N, M;
    cin >> N >> M;

    BinGraph graph(N);

    for (int i = 0; i < M; i++) {
        int x, y;
        cin >> x >> y;
        --x; --y;

        graph.add_edge(x, y);
    }

    int res = graph.primus();
    printf("%d\n", res);
    return 0;
}


int BinGraph::primus()
{
    auto distance_to_span   = vector<int>(neighbours.size(), 1);
    auto used               = vector<bool>(neighbours.size(), false);
    distance_to_span[0] = 0; // start with first

    // intiialuze unused nodes
    set<pair<int, int>> unused;
    for (int i = 0; i < (int)neighbours.size(); i++)
    {
        unused.insert(make_pair(distance_to_span[i], i));
    }

    while (!unused.empty())
    {
        int current = unused.begin()->second;
        unused.erase(unused.begin());
        used[current] = true;

        // look at vertexes
        for (int i = 0; i < (int)neighbours.size(); i++)
        {
            int d = 0;
            if (neighbours[current].find(i) != neighbours[current].end())
                d = 1;

            // if vertex is unused, update its distance to span
            if (!used[i] && distance_to_span[i] > d)
            {
                unused.erase({distance_to_span[i], i});
                distance_to_span[i] = d;
                unused.insert({distance_to_span[i], i});
            }
        }
    }

    int res = 0;
    for (int d : distance_to_span)
    {
        res += d;
    }
    return res;
}
