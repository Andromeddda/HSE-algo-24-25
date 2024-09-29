#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <assert.h>

#include <cctype>

// streams
#include <iostream>
#include <istream>
#include <ostream>

// containers
#include <string>
#include <vector>
#include <utility>

#define CHAR_TO_INT(c) ((digit_t)(c - '0'))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

using namespace std;

int
main(void) {
    string alley;
    cin >> alley;

    int count = 0;

    for (int i = 0; i < (int)alley.size(); i++) {
        if (alley[i] == '0')
            continue;

        int l = i - 1;
        int r = i + 1;

        while (l >= 0 && r < (int)alley.size()) {
            if (alley[l] == '1' && alley[r] == '1')
                count++;
            l--;
            r++;
        }
    }

    cout << count << '\n';
    return 0;
}