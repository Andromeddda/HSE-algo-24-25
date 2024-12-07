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
#include <thread>

using namespace std;

#define SIGNED
#ifdef SIGNED

    #define TIME_LIMIT 1440
    #define TRUCK_WEIGHT (3000000LL)
    #define CUP_WEIGHT (100LL)
    #define TOTAL_CUPS (10000000LL)

    typedef int minutes_t;
    typedef signed long long int cups_t;

#else

    #define TIME_LIMIT 1440
    #define TRUCK_WEIGHT (3000000LLU)
    #define CUP_WEIGHT (100LLU)
    #define TOTAL_CUPS (10000000LLU)

    typedef int minutes_t;
    typedef unsigned long long int cups_t;

#endif



struct Graph
{
    const int V;
    vector<vector<pair<int, pair<cups_t, minutes_t>>>> neighbours;
    // vector<set<pair<minutes_t, pair<int, cups_t>>>> neighbours;

    cups_t min_cups;
    cups_t max_cups;

    Graph() = delete;
    Graph(int v) : V(v), neighbours(v), min_cups(TOTAL_CUPS), max_cups(0) {}

    void add_edge(int from, int to, cups_t cups, minutes_t time) {
        neighbours[from].push_back(make_pair(to, make_pair(cups, time)));
        neighbours[to].push_back(make_pair(from, make_pair(cups, time)));
        // neighbours[from].insert(make_pair(time, make_pair(to, cups)));
        // neighbours[to].insert(make_pair(time, make_pair(from, cups)));

        min_cups = min(min_cups, cups);
        max_cups = max(max_cups, cups);
    }

    bool find_path(int from, int to, cups_t limit);
    cups_t traffic(int from, int to);
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


    cout << graph.traffic(0, Verticies - 1) << '\n';

    return 0;
}


bool Graph::find_path(int from, int to, cups_t limit)
{
    if (limit == 0LL) return true;
    // find shortest path (in minutes) to every node 
    // where each edge can hold at least <limit> cups 

    // return true if we can bring at least <limit> cups from <from> to <to>

    vector<minutes_t> time(V, TIME_LIMIT + 1);
    time[from] = 0;

    // Priority queue of relaxations
    set<pair<minutes_t, int>> relaxation_queue;

    relaxation_queue.insert(make_pair(0, from));

    while (!relaxation_queue.empty())
    {
        // Go to the closest undiscovered node
        auto It = relaxation_queue.begin();
        int current = It->second;
        relaxation_queue.erase(It);

        // Iterate through its neighbours
        for (auto& n : neighbours[current])
        {
            int neighbour = n.first;
            cups_t cup_limit = n.second.first;
            time_t road_time = n.second.second;

            // skip edges that cannot hold at least <limit> cups
            if (cup_limit < limit)
                continue;

            // Update distance to every neighbour
            if (time[neighbour] > time[current] + road_time) 
            {
                // erase old time-vertex pair
                relaxation_queue.erase({time[neighbour], neighbour});

                // update minimal time to neighbour
                time[neighbour] = time[current] + road_time;

                // insert new time-vertex pair
                relaxation_queue.insert({time[neighbour], neighbour});
            }
        }
    }


    return time[to] <= TIME_LIMIT;
}

cups_t Graph::traffic(int from, int to)
{
    // bin search
    cups_t low = 0;
    cups_t high = TOTAL_CUPS;
    cups_t res = 0;
    

    while (low <= high)
    {
        cups_t mid = (low + high) / 2;
        if (find_path(from, to, mid)) 
        {
            res = mid;
            low = mid + 1;
        }
        else 
            high = mid - 1;
    }

    return res;
}
