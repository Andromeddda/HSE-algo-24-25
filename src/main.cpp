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

typedef long long digit_t;
typedef vector<digit_t> Number;

const digit_t base = 1000000;
const digit_t char_base = 10;
const size_t digit_length = 6;
const size_t naive_size = 16;

// Console utils
pair<Number, bool> read_number(istream& is);
void               print_number(const Number& x, bool sign, ostream& os);

// Karatsuba
void                    normalize(Number& x);
Number                  Karatsuba(const Number& X, const Number& Y);
pair<digit_t, digit_t>  mod(digit_t dividor, digit_t divisor);

// Basic operations
Number subtract(const Number& x, const Number& y);
Number add(const Number& x, const Number& y);
Number naive_multiplication(const Number& x, const Number& y);

int 
main(void) {
    // test_karatsuba();
    pair<Number, bool> x = read_number(cin);
    pair<Number, bool> y = read_number(cin);
    print_number(Karatsuba(x.first, y.first), x.second ^ y.second, cout);
}


pair<Number, bool>
read_number(istream& is) {
    string str;
    is >> str;

    auto res = Number();

    int digit = 0;
    digit_t power = 1;

    // go from back
    for (int i = (int)str.size() - 1; i >= 0; i--) {
        if (!isdigit(str[i]))
            break;

        digit += CHAR_TO_INT(str[i]) * power;
        power *= char_base;

        if (power == base) {
            res.push_back(digit);
            digit = 0;
            power = 1;
        }
    }

    if (digit)
        res.push_back(digit);

    normalize(res);

    return pair<Number, bool>(res, str[0] == '-');
}


void
print_number(const Number& x, bool sign, ostream& os) {
    if(x.size() == 1 && x[0] == 0) {
        cout << "0\n";
        return;
    }
    // print the sign
    if (sign)
        os << '-';

    bool extend = false;

    int i = (int)x.size() - 1;

    // skip leading zeros
    while (!x[i])
        i--;

    for (; i >= 0; i--) {
        string str_digit = to_string(x[i]);
        if ((str_digit.size() < digit_length) && extend) {
            str_digit = string(digit_length - str_digit.size(), '0') + str_digit;
        }

        os << str_digit;
        extend = true;
    }
    cout << '\n';
}

Number
naive_multiplication(const Number& x, const Number& y) {
    // cases of zero
    if (!x.size() || !y.size())
        return Number(1, 0);
    if (x.size() == 1 && x[0] == 0)
        return Number(1, 0);
    if (y.size() == 1 && y[0] == 0)
        return Number(1, 0);

    size_t size = x.size() + y.size();

    auto res = Number(size, 0);
    for (auto i = 0LU; i < x.size(); ++i) {
        for (auto j = 0LU; j < y.size(); ++j) {
            res[i + j] += x[i] * y[j];
        }
    }

    normalize(res);
    return res;
}

void 
normalize(Number& x) {
    // normalize digits
    x.push_back(0);
    for (auto i = 0LU; i < x.size() - 1; ++i) {
        auto m = mod(x[i], base);
        x[i + 1] += m.first;
        x[i] = m.second;
    }

    // delete leading zeros
    while(!x[x.size() - 1])
        x.pop_back();
}

pair<digit_t, digit_t>
mod(digit_t dividor, digit_t divisor) {
    if (divisor < 0)
        exit(-1);

    auto res = 
        pair<digit_t, digit_t>(dividor / divisor, dividor % divisor);

    if (dividor < 0) {
        res.first--;
        res.second += divisor;
    }
    return res;
}

Number 
Karatsuba(const Number& X, const Number& Y) {
    Number x = Number(X);
    Number y = Number(Y);

    // cases of zero
    if (!x.size() || !y.size())
        return Number(1, 0);
    if (x.size() == 1 && x[0] == 0)
        return Number(1, 0);
    if (y.size() == 1 && y[0] == 0)
        return Number(1, 0);

    size_t size = MAX(x.size(), y.size());

    if (size < naive_size) {
        return naive_multiplication(x, y);
    }

    size_t k = size / 2;

    // Split vectors in two parts
    auto mid_x = (x.size() > k) ? (x.begin() + k) : x.end();
    auto mid_y = (y.size() > k) ? (y.begin() + k) : y.end();

    Number Xl {x.begin(), mid_x};
    Number Xr {mid_x, x.end()};
    Number Yl {y.begin(), mid_y};
    Number Yr {mid_y, y.end()};

    Number P1 = Karatsuba(Xl, Yl);
    Number P2 = Karatsuba(Xr, Yr);

    Number Xlr = add(Xl, Xr);
    Number Ylr = add(Yl, Yr);

    // Calculate P3
    Number P3 = subtract(subtract(Karatsuba(Xlr, Ylr), P1), P2);

    P2.insert(P2.begin(), 2*k, 0);
    P3.insert(P3.begin(), k, 0);

    Number res = add(add(P1, P3), P2);

    normalize(res);
    return res;
}

Number 
add(const Number& x, const Number& y) {
    size_t size = MAX(x.size(), y.size());
    auto res = Number();

    digit_t overload = 0;
    for (size_t i = 0; i < size; i++) {
        digit_t sum = overload;
        if (i < x.size()) sum += x[i];
        if (i < y.size()) sum += y[i];

        overload = sum / base;
        sum %= base;
        res.push_back(sum);
    }

    if (overload)
        res.push_back(overload);

    return res;
}

Number 
subtract(const Number& x, const Number& y) {
    size_t size = MAX(x.size(), y.size());

    Number res = Number();

    digit_t loan = 0;
    for (size_t i = 0; i < size; i++) {
        digit_t diff = -loan;
        if (i < x.size()) diff += x[i];
        if (i < y.size()) diff -= y[i];

        loan = 0;
        if (diff < 0) {
            loan = 1;
            diff += base;
        }

        res.push_back(diff);
    }

    while (!res[res.size() - 1])
        res.pop_back();

    return res;
}
