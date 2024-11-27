#include <iostream>
#include <fstream>

#include <utility>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <random>

#include <cstdlib>
#include <math.h>
#include <assert.h>

using namespace std;

struct DSU
{
    vector<int> nodes;
    int total;

    DSU(int N);
    ~DSU() = default;

    int find(int index);
    void unite(int a, int b);

};

int main(void)
{
    // N - numner of vertexes
    // M - number of edges
    int N, M;
    cin >> N >> M;

    list<pair<int, int>>    edges_additions;
    unordered_map<int, list<pair<int, int>>::const_iterator>
                            query_positions;

    // Read M edges in the initial graph
    for (int i = 0; i < M; i++)
    {
        int a, b;
        cin >> a >> b;
        --a, --b;

        edges_additions.push_front(make_pair(a, b));
        query_positions[i] = edges_additions.begin();
    }

    // Q - number of edge deletions
    int Q;
    cin >> Q;

    list<int> edges_deletions;

    // Read number of edges to delete
    for (int i = 0; i < Q; i++)
    {
        int a;
        cin >> a;
        --a;
        edges_deletions.push_back(a);
    }

    // Rewrite the history of additions
    for (int i = 0; i < Q; i++)
    {
        int to_delete = edges_deletions.back();
        edges_deletions.pop_back();

        auto pos = query_positions.at(to_delete);
        edges_additions.push_back(*pos);
        edges_additions.erase(pos);
    }

    DSU dsu(N);
    vector<int> answers;

    // Add all edges and count components at each step
    for (int i = M; i > 0; i--)
    {
        answers.push_back(dsu.total);
        pair<int, int> to_add = edges_additions.front();
        edges_additions.pop_front();

        dsu.unite(to_add.first, to_add.second);
    }

    for (int i = 1; i <= Q; i++)
        cout << answers[answers.size() - i] << ' ';
    
    cout << '\n';
}

DSU::DSU(int N) : nodes(N), total(N) { 
    for (int i = 0; i < N; i++)
        nodes[i] = i;
}


int DSU::find(int index) {
    if (nodes[index] == index) return index;
    return nodes[index] = find(nodes[index]);
}

void DSU::unite(int a, int b) {
    int a_parent = find(a);
    int b_parent = find(b);

    if (a_parent == b_parent) return;

    if (rand() % 2 == 0)
        nodes[a_parent] = b_parent;
    else
        nodes[b_parent] = a_parent;

    total--;
}