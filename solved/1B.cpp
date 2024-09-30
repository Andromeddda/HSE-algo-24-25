// #include <cstdio>
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

#define CHAR_TO_INT(c) ((digit_t)(c - '0'))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

using namespace std;

typedef signed long long digit_t;
typedef vector<digit_t> Polynom;

const size_t naive_size = 64;

Polynom make_vec_from_str(const string& s);
void print_vector(const Polynom& vec);

Polynom                  Karatsuba(const Polynom& X, const Polynom& Y);
pair<digit_t, digit_t>  mod(digit_t dividor, digit_t divisor);

// Basic operations
Polynom subtract(const Polynom& x, const Polynom& y);
Polynom add(const Polynom& x, const Polynom& y);
Polynom naive_multiplication(const Polynom& x, const Polynom& y);

ssize_t count_ways(const Polynom& x);

int 
main(void) 
{
    string alley;
    cin >> alley;

    Polynom x = make_vec_from_str(alley);
    cout << count_ways(x) << '\n';
    
}

Polynom
naive_multiplication(const Polynom& x, const Polynom& y) 
{
    // cases of zero
    if (!x.size() || !y.size())
        return Polynom(1, 0);
    if (x.size() == 1 && x[0] == 0)
        return Polynom(1, 0);
    if (y.size() == 1 && y[0] == 0)
        return Polynom(1, 0);

    size_t size = x.size() + y.size();

    auto res = Polynom(size, 0);
    for (auto i = 0LU; i < x.size(); ++i) {
        for (auto j = 0LU; j < y.size(); ++j) {
            res[i + j] += x[i] * y[j];
        }
    }
    return res;
}


Polynom 
Karatsuba(const Polynom& X, const Polynom& Y) 
{
    Polynom x = Polynom(X);
    Polynom y = Polynom(Y);

    // cases of zero
    if (!x.size() || !y.size())
        return Polynom(1, 0);
    if (x.size() == 1 && x[0] == 0)
        return Polynom(1, 0);
    if (y.size() == 1 && y[0] == 0)
        return Polynom(1, 0);

    size_t size = MAX(x.size(), y.size());

    if (size < naive_size) {
        return naive_multiplication(x, y);
    }

    size_t k = size / 2;

    // Split vectors in two parts
    auto mid_x = (x.size() > k) ? (x.begin() + k) : x.end();
    auto mid_y = (y.size() > k) ? (y.begin() + k) : y.end();

    Polynom Xl {x.begin(), mid_x};
    Polynom Xr {mid_x, x.end()};
    Polynom Yl {y.begin(), mid_y};
    Polynom Yr {mid_y, y.end()};

    Polynom P1 = Karatsuba(Xl, Yl);
    Polynom P2 = Karatsuba(Xr, Yr);

    Polynom Xlr = add(Xl, Xr);
    Polynom Ylr = add(Yl, Yr);

    // Calculate P3
    Polynom P3 = subtract(subtract(Karatsuba(Xlr, Ylr), P1), P2);

    P2.insert(P2.begin(), 2*k, 0);
    P3.insert(P3.begin(), k, 0);

    Polynom res = add(add(P1, P3), P2);
    return res;
}

Polynom 
add(const Polynom& x, const Polynom& y) 
{
    size_t size = MAX(x.size(), y.size());
    auto res = Polynom();
    for (size_t i = 0; i < size; i++) {
        digit_t sum = 0;
        if (i < x.size()) sum += x[i];
        if (i < y.size()) sum += y[i];

        res.push_back(sum);
    }
    return res;
}

Polynom 
subtract(const Polynom& x, const Polynom& y) 
{
    size_t size = MAX(x.size(), y.size());

    Polynom res = Polynom();
    for (size_t i = 0; i < size; i++) {
        digit_t diff = 0;
        if (i < x.size()) diff += x[i];
        if (i < y.size()) diff -= y[i];
        res.push_back(diff);
    }

    return res;
}

Polynom 
make_vec_from_str(const string& s) 
{
    auto res = Polynom();

    for (char c : s) {
        res.push_back((digit_t)(c - '0'));
    }
    return res;
}


void 
print_vector(const Polynom& vec) 
{
    cout << "{ ";

    for (auto i = 0LU; i < vec.size() - 1; i++) {
        cout << vec[i] << ", ";
    }

    cout << vec[vec.size() - 1] << " }\n";
}

ssize_t 
count_ways(const Polynom& x) 
{
    ssize_t ways = 0;
    Polynom xx = Karatsuba(x, x);

    // print_vector(x);
    // print_vector(xx);

    for (auto i = 0LU; i < xx.size(); i += 2) 
    {
        if (xx[i] < (digit_t)2)
            continue;

        if (!x[i / 2])
            continue;

        ways += (ssize_t)xx[i] - 1;
    }
    return ways / 2;
}