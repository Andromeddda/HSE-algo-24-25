#include <iostream>
#include <fstream>
#include <string>

#include <algorithm>
// #include <random>

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

#define TIME_LIMIT 1440
#define TRUCK_WEIGHT (3000000LLU)
#define CUP_WEIGHT (100LLU)
#define TOTAL_CUPS (10000000LLU)

typedef int minutes_t;
typedef unsigned long long int cups_t;

struct Graph
{
    const int V;
    // vector<vector<pair<int, pair<cups_t, minutes_t>>>> neighbours;
    vector<set<pair<minutes_t, pair<int, cups_t>>>> neighbours;

    Graph() = delete;
    Graph(int v) : V(v), neighbours(v) {}

    void add_edge(int from, int to, cups_t cups, minutes_t time) {
        // neighbours[from].push_back(make_pair(to, make_pair(cups, time)));
        // neighbours[to].push_back(make_pair(from, make_pair(cups, time)));
        neighbours[from].insert(make_pair(time, make_pair(to, cups)));
        neighbours[to].insert(make_pair(time, make_pair(from, cups)));
    }

    vector<cups_t> Dijkstra(int root);
};


int main(void) 
{
    int Verticies, Edges;
    cin >> Verticies >> Edges;

    Graph graph(Verticies);

    for (int i = 0; i < Edges; i++)
    {
        int v1, v2;
        minutes_t time;
        cups_t weight;

        cin >> v1 >> v2 >> time >> weight;
        --v1;
        --v2;

        // calculate cups from weight
        cups_t cups;
        if (weight <= TRUCK_WEIGHT) 
            cups = 0;
        else
            cups = (weight - TRUCK_WEIGHT) / CUP_WEIGHT;

        // add edge
        graph.add_edge(v1, v2, cups, time);
    }

    auto d = graph.Dijkstra(0);

    cout << d[Verticies - 1] << '\n';

    return 0;
}


vector<cups_t> Graph::Dijkstra(int root)
{
    // result - max cups from root to vertex
    vector<cups_t> cups(V, 0);
    cups[root] = TOTAL_CUPS;

    vector<minutes_t> time(V, TIME_LIMIT + 1);
    time[root] = 0;

    // Priority queue of relaxations
    set<pair<minutes_t, pair<cups_t, int>>> relaxation_queue;

    relaxation_queue.insert(make_pair(0, make_pair(TOTAL_CUPS, root)));
    

    while (!relaxation_queue.empty())
    {
        // Go to the road with max weight limit
        auto It = relaxation_queue.begin();
        int current = It->second.second;
        relaxation_queue.erase(It);

        // Iterate through its neighbours
        // for (auto i = 0LU; i < neighbours[current].size(); i++)
        for (auto& n : neighbours[current])
        {
            int neighbour = n.second.first;
            cups_t cup_limit = n.second.second;
            time_t road_time = n.first;

            time_t time_if_we_update_this_road = time[current] + road_time; 

            if (time_if_we_update_this_road > TIME_LIMIT)
                continue;

            // Update distance to every neighbour
            if (cups[neighbour] <= min(cups[current], cup_limit)) 
            {
                // erase old distance-vertex pair
                relaxation_queue.erase({time[neighbour], {cups[neighbour], neighbour}});

                // update distance
                cups[neighbour] = min(cups[current], cup_limit);
                time[neighbour] = time_if_we_update_this_road;

                // insert new distance-vertex pair
                relaxation_queue.insert({time[neighbour], {cups[neighbour], neighbour}});

            }
        }
    }


    for (int i = 0; i < V; i++)
        if (time[i] > TIME_LIMIT)
            cups[i] = 0;

    return cups;
}