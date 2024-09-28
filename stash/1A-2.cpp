#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <assert.h>

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#define CHAR_TO_INT(c) ((digit_t)(c - '0'))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define KARATSUBA_MIN_LEN 10

typedef unsigned long long digit_t;
#define BASE 10000000000000000000LLU
#define BASE_DIGITS 19

using namespace std;

vector<digit_t> 
Karatsuba(const vector<digit_t>& x, const vector<digit_t>& y);

vector<digit_t> 
naive_multiplication(const vector<digit_t>& x, const vector<digit_t>& y);

vector<digit_t> 
add_vectors(const vector<digit_t>& x, const vector<digit_t>& y);

vector<digit_t> 
subtract_vectors(const vector<digit_t>& x, const vector<digit_t>& y);

int main(void) {
	vector<digit_t> x, y;

	string x_str, y_str;
	cin >> x_str >> y_str;

	bool sgn = false;
	size_t i = 0, j = 0;

	// process sign of production
	if (x_str[0] == '-') {
		sgn = !sgn;
		i++;
	}
	if (y_str[0] == '-') {
		sgn = !sgn;
		j++;
	}


	for (; i < x_str.size(); i++) {
		x.push_back(CHAR_TO_INT(x_str[i]));
	}

	for (; j < y_str.size(); j++)
		y.push_back(CHAR_TO_INT(y_str[j]));

	vector<digit_t> res = Karatsuba(x, y);

	if (sgn)
		cout << '-';

	for (size_t i = 0; i < res.size(); i++) {
		if (!i) {
			cout << res[i];
			continue;
		}

		string main = to_string(res[i]);
		string zeros = string(BASE_DIGITS - main.size(), '0');

		cout << zeros << main;
	}

	return 0;
}

// Recursive multiplication of two vectors
vector<digit_t> 
Karatsuba(const vector<digit_t>& x, const vector<digit_t>& y) {
	size_t size = MAX(x.size(), y.size());

	if (size < KARATSUBA_MIN_LEN) {
		return naive_multiplication(x, y);
	}

	size_t k = size / 2;

	// Split vectors in two parts
	auto mid_x = (x.size() > k) ? (x.end() - k) : x.begin();
	auto mid_y = (y.size() > k) ? (y.end() - k) : y.begin();

	vector<digit_t> Xl {x.begin(), mid_x};
	vector<digit_t> Xr {mid_x, x.end()};
	vector<digit_t> Yl {y.begin(), mid_y};
	vector<digit_t> Yr {mid_y, y.end()};

	vector<digit_t> P1 = Karatsuba(Xl, Yl);
	vector<digit_t> P2 = Karatsuba(Xr, Yr);

	vector<digit_t> Xlr = add_vectors(Xl, Xr);
	vector<digit_t> Ylr = add_vectors(Yl, Yr);

	// Calculate P3
	vector<digit_t> P3 = subtract_vectors(subtract_vectors(Karatsuba(Xlr, Ylr), P1), P2);

	// P1 = P1*qq
	// P3 = P3*q
	P1.insert(P1.end(), 2*k, 0);
	P3.insert(P3.end(), k, 0);

	vector<digit_t> result = add_vectors(add_vectors(P1, P3), P2);

	return result;
}

// Multiply two vector<digit_t> with complixity O(n^2)
vector<digit_t> 
naive_multiplication(const vector<digit_t>& x, const vector<digit_t>& y) {

	if (MIN(x.size(), y.size()) == 0) return vector<digit_t>(1, 0);

	// Create new vector<digit_t>
	size_t size = x.size() + y.size();
	vector<digit_t> result = vector<digit_t>(size, 0);

	// Run through and add
	for (int i = x.size() - 1; i >= 0; --i) {
		for (int j = y.size() - 1; j >= 0; --j) {
			result[i + j + 1] += x[i]*y[j];
		}
	}
	// fix digit overflow
	for (int i = result.size() - 1; i > 0; --i) {
		result[i - 1] += result[i] / 10;
		result[i] %= 10;
	}

	while (result.size() > 1 && result[0] == 0) {
		result.erase(result.begin());
	}

	return result;
}

// Overload addition of vectors
vector<digit_t> 
add_vectors(const vector<digit_t>& x, const vector<digit_t>& y) {
	// Copy
	vector<digit_t> d1 = vector<digit_t>(x);
	vector<digit_t> d2 = vector<digit_t>(y);

	size_t size = MAX(d1.size(), d2.size());

	// Insert zeros in the front
    if (d1.size() < size)
    	d1.insert(d1.begin(), size - d1.size(), 0);
    if (d2.size() < size)
    	d2.insert(d2.begin(), size - d2.size(), 0);

    // Create new vector
    vector<digit_t> result = vector<digit_t>(1 + size, 0);

    // Go from the back and add
    digit_t overflow = 0, sum = 0;
    for (int i = size - 1; i >= 0; i--) {
    	sum = d1[i] + d2[i] + overflow;

    	// Add with shift to i+1 because of extra digit, insertet in the front
    	result[i+1] = sum % 10; 
    	overflow = sum / 10;
    }

    // Insert or pop the first digit
    if (overflow != 0) {
    	result[0] = overflow;
    }
    else {
    	result.erase(result.begin());
    }

	return result;
}

// Overload subtraction of vectors
vector<digit_t> 
subtract_vectors(const vector<digit_t>& x, const vector<digit_t>& y) {
	// Copy
	vector<digit_t> d1 = vector<digit_t>(x);
	vector<digit_t> d2 = vector<digit_t>(y);

	size_t size = MAX(d1.size(), d2.size());

	// Insert zeros in the front
    if (d1.size() < size)
    	d1.insert(d1.begin(), size - d1.size(), 0);
    if (d2.size() < size)
    	d2.insert(d2.begin(), size - d2.size(), 0);

    // Create new vector
    vector<digit_t> result = vector<digit_t>(size, 0);

    // Run and subtract
    digit_t loan = 0; 
    for (int i = size - 1; i >= 0; i--) {
    	if (d1[i] - loan < d2[i]) {
    		// Need to take extra 10 from next digit
    		result[i] = 10 + d1[i] - loan - d2[i];
    		loan = 1;
    	}
    	else {
    		result[i] = d1[i] - loan - d2[i];
    		loan = 0;
    	}
    }

    // Remove first digit if it is 0
    if (result[0] == 0) {
    	result.erase(result.begin());
    }

	return result;
}