#include <cstdio>
#include <cstdlib>
// #include <cstring>
// #include <assert.h>

#include <cctype>

// streams
#include <iostream>
#include <istream>
#include <ostream>

// containers
#include <string>
#include <vector>
#include <utility>

// random
#include <random>

// math
#include <math.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

using namespace std;

void play(int n);

int 
main(void) 
{
    int m;
    cin >> m;

    for (int i = 0; i < m; i++) {
        play(i + 2);
    }

    return 0;
}

void play(int n) {
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            printf("? %d %d\n", i, j);
        }
    }
    printf("! %d %d\n", n / 2, n / 2 + 1);
}

