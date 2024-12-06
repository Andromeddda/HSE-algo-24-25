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
#include <array>

#include <utility>
#include <exception>

#include <cstdlib>
#include <math.h>
#include <assert.h>
#include <climits>

using namespace std;

#define INF 1441
#define LIMIT 1440


struct Path
{
    int to, cups, time;
    Path(int v, int c, int t) : to(v), cups(c), time(t) {}

    Path() : to(-1), cups(0), time(INF) {}
    Path(const Path& other) = default;
    Path(Path&& other) = default;
    Path& operator=(const Path& other) = default;
    Path& operator=(Path&& other) = default;

    bool operator== (const Path& other) {
        return to == other.to && cups == other.cups;
    }
};

bool operator< (const Path& lhs, const Path& rhs) { 
    if (lhs.time > LIMIT) return false;
    if (rhs.time > LIMIT) return true;
    return lhs.cups > rhs.cups;
}

struct Graph
{
    const int V;
    vector<vector<array<int, 3>>> neighbours;

    Graph() = delete;
    Graph(int v) : V(v), neighbours(v) {}

    void add_edge(int from, int to, int cups, int time) {
        array<int, 3> first {{to, cups, time}};
        array<int, 3> second {{from, cups, time}};
        neighbours[from].push_back(first);
        neighbours[to].push_back(second);
    }

    vector<int> Dijkstra(int root);
};


int main(void) 
{
    int Verticies, Edges;
    cin >> Verticies >> Edges;

    Graph graph(Verticies);

    for (int i = 0; i < Edges; i++)
    {
        int v1, v2, time, weight;
        cin >> v1 >> v2 >> time >> weight;
        --v1;
        --v2;

        // calculate cups from weight
        int cups;
        if (weight < 3000000) 
            cups = 0;
        else
            cups = (weight - 3000000) / 100;

        graph.add_edge(v1, v2, cups, time);
    }

    auto d = graph.Dijkstra(0);

    cout << d[Verticies - 1] << '\n';

    return 0;
}


vector<int> Graph::Dijkstra(int root)
{
    // result - max cups from root to vertex
    vector<int> distances(V, 0);
    distances[root] = 10000000;

    // Priority queue of relaxations
    set<Path> relaxation_queue;

    relaxation_queue.insert(Path{root, 10000000, 0});

    while (!relaxation_queue.empty())
    {
        // Go to the nearest vertex and erase it from queue
        int current = relaxation_queue.begin()->to;
        int cur_time = relaxation_queue.begin()->time;

        relaxation_queue.erase(relaxation_queue.begin());

        if (cur_time > LIMIT)
            continue;

        // Iterate through its neighbours
        for (auto i = 0LU; i < neighbours[current].size(); i++)
        {
            int neighbour = neighbours[current][i][0];
            int cups = neighbours[current][i][1];
            int time = neighbours[current][i][2];

            if (time + cur_time > LIMIT)
                continue;

            // Update distance to every neighbour
            if (distances[neighbour] < min(distances[current], cups)) 
            {
                // erase old distance-vertex pair
                relaxation_queue.erase(Path(neighbour, distances[neighbour], INF));

                // update distance
                distances[neighbour] = min(distances[current], cups);

                // insert new distance-vertex pair
                relaxation_queue.insert(Path(neighbour, distances[neighbour], time + cur_time));
            }
        }
    }

    return distances;
}