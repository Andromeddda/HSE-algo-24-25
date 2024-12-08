// I/O
#include <iostream>
#include <fstream>
#include <cstdio>

// containers
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <list>
#include <utility>
#include <stack>

// runtime
#include <exception>
#include <assert.h>

// other
#include <algorithm>
#include <random>
#include <cstdlib>
#include <climits>
#include <math.h>
#include <memory>

using namespace std;

#define INF (INT_MAX)

class Graph
{
private:
    int V;
    int* matrix;
    int* color;
public:
    Graph(int N) 
    : V(N)
    { 
        matrix = new int[N*N];

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                if (i != j)
                    matrix[N*i + j] = 0;
    }

    inline ~Graph() { delete[] matrix; }

    void add_edge(int from, int to, int weight) 
    {
        matrix[V*from + to] = weight;
    }

    int cyclic()
    {
        color = new int[V];
        for (int i = 0; i < V; i++)
            color[i] = 0;

        for (int i = 0; i < V; i++)
        {
            if (color[i] == 0)
                if (dfs(i))
                    return true;
        }
        return false;
    }

private:
    bool dfs(int root)
    {
        color[root] = 1;

        for (int i = 0; i < V; i++)
            if (matrix[V*root + i] == 1)
            {
                if (color[i] == 0)
                {
                    if (dfs(i))
                        return true;
                }
                if (color[i] == 1)
                    return true;
            }

        color[root] = 2;
        return false;
    }
};


int main(void) 
{
    int V;
    cin >> V;

    Graph graph(V);

    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
        {
            int w;
            cin >> w;
            graph.add_edge(i, j, w);
        }

    cout << graph.cyclic() << '\n';

    return 0;
}

