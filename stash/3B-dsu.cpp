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
    vector<int>     nodes;
    vector<int>     sum;
    vector<bool>    exist;

    int max_sum = 0;

    DSU(int N);
    ~DSU() = default;

    int find(int index);
    void draw(int index, int value);
    void unite(int a, int b);

};

int main(void) 
{
    int N;
    cin >> N;

    auto values = vector<int>(N, 0);
    auto positions = vector<int>(N, 0);

    for (int i = 0; i < N; i++)
    {
        int a;
        cin >> a;
        values[i] = a;
    }

    for (int i = 0; i < N; i++)
    {
        int a;
        cin >> a;
        positions[i] = a - 1;
    }

    DSU dsu(N);

    list<int> answers;



    for (int i = N - 1; i >= 0; i--)
    {
        answers.push_back(dsu.max_sum);
        dsu.draw(positions[i], values[positions[i]]);
    }

    while (!answers.empty()) {
        cout << answers.back() << ' ';
        answers.pop_back();
    }

    return 0;
}

DSU::DSU(int N) 
: nodes(N, 0), sum(N, 0), exist(N, false) 
{ 
    for (int i = 0; i < N; i++)
        nodes[i] = i;
}


int DSU::find(int index) {
    if (nodes[index] == index) return index;

    nodes[index] = find(nodes[index]);
    sum[index] = sum[nodes[index]];

    return nodes[index];
}

void DSU::unite(int a, int b) {
    int a_parent = find(a);
    int b_parent = find(b);

    if (a_parent == b_parent) return;

    if (rand() % 2 == 0) {
        nodes[a_parent] = b_parent;
        sum[b_parent] += sum[a_parent];

        max_sum = max(sum[b_parent], max_sum);
    }
    else {
        nodes[b_parent] = a_parent;
        sum[a_parent] += sum[b_parent];

        max_sum = max(sum[a_parent], max_sum);
    }
}

void DSU::draw(int index, int value)
{
    exist[index] = true;
    sum[index] = value;

    if (index > 0)
        if (exist[index - 1])
            unite(index - 1, index);

    if (index < (int)nodes.size() - 1)
        if (exist[index + 1])
            unite(index + 1, index);

    max_sum = max(max_sum, sum[index]);
}