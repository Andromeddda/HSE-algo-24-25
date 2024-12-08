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

// runtime
#include <exception>
#include <assert.h>

// other
#include <algorithm>
#include <random>
#include <cstdlib>
#include <climits>
#include <math.h>

using namespace std;

#define INF (INT_MAX)

class Graph
{
private:
    int V;
    int* floyd;
public:
    Graph(int N) 
    : V(N) 
    { 
        floyd = new int[N*N];

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                if (i != j)
                    floyd[N*i + j] = INF;
    }

    inline ~Graph() { delete[] floyd; }

    void add_edge(int from, int to, int weight) 
    {
        floyd[V*from + to] = weight;
    }

    void Floyd()
    {
        for (int k = 0; k < V; k++)
            for (int i = 0; i < V; i++)
                for (int j = 0; j < V; j++)
                    if (floyd[V*i + j] > floyd[V*i + k] + floyd[V*k + j])
                        floyd[V*i + j] = floyd[V*i + k] + floyd[V*k + j];
    }


    int max_shortest_path()
    {
        int res = 0;
        for (int i = 0; i < V; i++)
                for (int j = 0; j < V; j++)
                    if (res < floyd[V*i + j])
                        res = floyd[V*i + j];
        return res;
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

    graph.Floyd();

    cout << graph.max_shortest_path() << '\n';
    return 0;
}

