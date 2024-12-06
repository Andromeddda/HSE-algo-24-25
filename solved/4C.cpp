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
#include <climits>

using namespace std;

#define INF INT_MAX

struct Graph
{
    const int V;
    vector<vector<pair<int, pair<int, int>>>> neighbours;

    Graph() = delete;
    Graph(int v) : V(v), neighbours(v) {}

    void add_edge(int from, int to, int departure, int arrival) 
    {
        neighbours[from].push_back(make_pair(to, make_pair(departure, arrival)));
    }

    vector<int> Dijkstra(int root);
};


int main(void) 
{
    int Verticies, Edges, Start, Finish;
    cin >> Verticies >> Start >> Finish >> Edges;
    --Start;
    --Finish;

    Graph graph(Verticies);

    for (int i = 0; i < Edges; i++)
    {
        int v1, v2, d, a;
        cin >> v1 >> d >> v2 >> a;
        --v1;
        --v2;
        graph.add_edge(v1, v2, d, a);
    }

    vector<int> dists = graph.Dijkstra(Start);


    if (dists[Finish] != INF)
        cout << dists[Finish] << '\n';
    else
        cout << -1 << '\n';

    return 0;
}


vector<int> Graph::Dijkstra(int root)
{
    // result - shortest distances to each vertex from s
    vector<int> distances(V, INF);
    distances[root] = 0;

    // Priority queue of relaxations
    // time -> vertex
    set<pair<int, int>> relaxation_queue;
    relaxation_queue.insert(make_pair(0, root));

    while (!relaxation_queue.empty())
    {
        // Go to the nearest vertex and erase it from queue
        int current = relaxation_queue.begin()->second;
        relaxation_queue.erase(relaxation_queue.begin());

        // Iterate through its neighbours
        for (auto i = 0LU; i < neighbours[current].size(); i++)
        {
            int neighbour = neighbours[current][i].first;
            int departure = neighbours[current][i].second.first;
            int arrival   = neighbours[current][i].second.second;

            // Update distance to every neighbour
            if ((distances[neighbour] > arrival) && (distances[current] <= departure))
            {
                // erase old distance-vertex pair
                relaxation_queue.erase(make_pair(distances[neighbour], neighbour));

                // update distance
                distances[neighbour] = arrival;

                // insert new distance-vertex pair
                relaxation_queue.insert(make_pair(distances[neighbour], neighbour));
            }
        }
    }

    return distances;
}