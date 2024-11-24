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

template <typename A, typename B>
ostream& operator<< (ostream& os, const pair<A, B> p)
{
    os << "[" << p.first << ", " << p.second << "]";
    return os;
}


template <typename T>
ostream& operator<< (ostream& os, const list<T> l)
{
    for (T t : l)
        os << t << " -> ";
    os << "end\n";
    return os;
}


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
    int N, M;
    cin >> N >> M;


    list<pair<int, int>>    edges_additions;
    unordered_map<int, list<pair<int, int>>::const_iterator>
                            query_positions;

    for (int i = 0; i < M; i++)
    {
        int a, b;
        cin >> a >> b;

        edges_additions.push_front(make_pair(a - 1, b - 1));
        query_positions[i] = edges_additions.begin();
    }

    cout << edges_additions;

    int Q;
    cin >> Q;

    list<int> edges_deletions;

    for (int i = 0; i < Q; i++)
    {
        int a;
        cin >> a;
        edges_deletions.push_back(a - 1);
    }

    cout << edges_deletions;

    for (int i = 0; i < Q; i++)
    {
        int to_delete = edges_deletions.back();
        edges_deletions.pop_back();

        auto pos = query_positions.at(to_delete);
        edges_additions.push_back(*pos);
        edges_additions.erase(pos);
    }

    cout << edges_additions;

    DSU dsu(N);
    list<int> answers;
    answers.push_front(0);
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
        nodes[a] = b_parent;
    else
        nodes[b] = a_parent;

    total--;
}