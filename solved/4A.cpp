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
    vector<vector<pair<int, int>>> neighbours;

    Graph() = delete;
    Graph(int v) : V(v), neighbours(v) {}

    void add_edge(int from, int to, int length) {
        neighbours[from].push_back(make_pair(to, length));
        neighbours[to].push_back(make_pair(from, length));
    }

    vector<int> Dijkstra(int root);

    list<int> Path(int start, int finish, vector<int>& dijkstra);
};


int main(void) 
{
    int Verticies, Edges, Start, Finish;
    cin >> Verticies >> Edges >> Start >> Finish;

    Graph graph(Verticies);

    for (int i = 0; i < Edges; i++)
    {
        int v1, v2, weight;
        cin >> v1 >> v2 >> weight;
        graph.add_edge(v1, v2, weight);
    }


    vector<int> dists = graph.Dijkstra(Start);

    for (int &d : dists)
    {
        if (d == INF)
            d = -1;
        cout << d << ' ';
    }
    cout << '\n';
    
    // no path
    if (dists[Finish] == -1)
    {
        cout << -1;
        return 0;
    }

    list<int> path = graph.Path(Start, Finish, dists);

    cout << path.size() << '\n';

    for (int v : path)
        cout << v << ' ';
    cout << '\n';


    return 0;
}


vector<int> Graph::Dijkstra(int root)
{
    // result - shortest distances to each vertex from s
    vector<int> distances(V, INF);
    distances[root] = 0;

    // Priority queue of relaxations
    set<pair<int, int>> relaxation_queue;
    relaxation_queue.insert({0, root});

    while (!relaxation_queue.empty())
    {
        // Go to the nearest vertex and erase it from queue
        int current = relaxation_queue.begin()->second;
        relaxation_queue.erase(relaxation_queue.begin());

        // Iterate through its neighbours
        for (auto i = 0LU; i < neighbours[current].size(); i++)
        {
            int neighbour = neighbours[current][i].first;
            int weight = neighbours[current][i].second;
            // Update distance to every neighbour
            if (distances[neighbour] > distances[current] + weight) 
            {
                // erase old distance-vertex pair
                relaxation_queue.erase(make_pair(distances[neighbour], neighbour));

                // update distance
                distances[neighbour] = distances[current] + weight;

                // insert new distance-vertex pair
                relaxation_queue.insert(make_pair(distances[neighbour], neighbour));
            }
        }
    }

    return distances;
}


list<int> Graph::Path(int start, int finish, vector<int>& dijkstra)
{
    list<int> path;

    if (dijkstra[finish] == -1 || dijkstra[finish] == INF)
        return path;

    int current = finish;
    while (current != start) 
    {
        for (auto i = 0LU; i < neighbours[current].size(); i++)
        {
            int neighbour = neighbours[current][i].first;
            int weight = neighbours[current][i].second;

            if (dijkstra[neighbour] + weight == dijkstra[current])
            {
                path.push_front(current);
                current = neighbour;
                break;
            }
        }
    }

    path.push_front(start);

    return path;
}