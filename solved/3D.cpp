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

typedef long long int_t;

struct DSU
{
    vector<int_t> nodes;

    DSU(int_t N);
    ~DSU() = default;

    int_t find(int_t index);
    bool unite(int_t a, int_t b);

};

int_t solution(int_t K);


int main(void)
{
    int_t k;
    cin >> k;
    cout << solution(k);

    return 0;
}

int_t solution(int_t K) 
{
    DSU dsu(1);

    int_t n = 2;
    int_t find = true;

    unordered_map<int_t, unordered_set<int_t>> squares;

    while (n <= 4*K)
    {
        dsu.nodes.push_back(n - 1);

        unordered_set<int_t> united_with;

        int_t start = K - (n*n % K);

        // all numbers i such that (i + n*n) % K == 0
        for (int_t i = start; i < n; i += K) {
            find &= dsu.unite(i - 1, n - 1);
            united_with.insert(i);
        }

        // all numbers i such that (i*i + n) % K == 0
        int_t remainder = K - (n % K);
        for (int_t i : squares[remainder]) {
            if (united_with.find(i) == united_with.end())     // avoid double uniting with same numbers
                find &= dsu.unite(i - 1, n - 1);
        }

        if (!find)
            return n;

        squares[n*n % K].insert(n);
        n++;
    }

    return -1;
}

DSU::DSU(int_t N) : nodes(N) { 
    for (int_t i = 0; i < N; i++)
        nodes[i] = i;
}


int_t DSU::find(int_t index) {
    if (nodes[index] == index) 
        return index;

    nodes[index] = find(nodes[index]);
    return nodes[index];
}

bool DSU::unite(int_t a, int_t b) {
    int_t a_parent = find(a);
    int_t b_parent = find(b);

    if (a_parent == b_parent) 
        return false;

    if (rand() % 2 == 0)
        nodes[a_parent] = b_parent;
    else
        nodes[b_parent] = a_parent;

    return true;
}
