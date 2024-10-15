#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <random>
#include <math.h>
#include <assert.h>

using namespace std;

typedef long double coord_t;

const coord_t pi = 3.141592653589793238462643383279;
const coord_t smile_space = 20106.1929829747;

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

struct Point {
    coord_t x;
    coord_t y;
    Point(coord_t X, coord_t Y) : x(X), y(Y) {}
    Point(const Point &other) = default;
    Point(Point &&other) = default;
    Point& operator= (const Point &other) = default;
    Point& operator= (Point &&other) = default;
    // bool   operator== (const Point &other) const = default;
};


/*
 * Given two sets of 1-dimensional segments, ordered by left end
 * The function returns the union of these two sets, also ordered by segment's left end
*/
vector<pair<coord_t, coord_t>> 
combine_segments(vector<pair<coord_t, coord_t>> A, vector<pair<coord_t, coord_t>> B);

pair<coord_t, coord_t>
intersect_with_circle(Point O, coord_t R, coord_t x);

vector<pair<coord_t, coord_t>>
intersect_with_smile(Point O, coord_t x);

coord_t
sum_length(vector<pair<coord_t, coord_t>> A);

coord_t 
distance(Point A, Point B);

coord_t 
slice(Point O1, Point O2, coord_t x);

coord_t
calculate_space(Point O1, Point O2, coord_t E);

coord_t
calculate_space_recursive(Point O1, Point O2, coord_t a, coord_t b, coord_t E);

int main(void)
{
    coord_t x1, x2, y1, y2;
    cin >> x1 >> y1 >> x2 >> y2;

    Point O1(x1, y1);
    Point O2(x2, y2);

    coord_t S = calculate_space(O1, O2, 0.0001L);

    printf("%.5Lf\n", S);

    return 0;
}

/*
 * Given two sets of 1-dimensional segments, ordered by left end
 * The function returns the union of these two sets, also ordered by segment's left end
*/
vector<pair<coord_t, coord_t>> 
combine_segments(vector<pair<coord_t, coord_t>> A, vector<pair<coord_t, coord_t>> B) {
    // If one of sets is empty, return other set
    if (!A.size()) return B;
    if (!B.size()) return A;

    // Initializ the result
    vector<pair<coord_t, coord_t>> res;

    // i for index in A, 
    // j for index in B, 
    // k for index in res
    size_t i = 0, j = 0, k = 0;

    // Find leftmost segment and push it to the result
    if (A[i].first < B[j].first)
        res.push_back(A[i++]);
    else
        res.push_back(B[j++]);

    // iterate until one of the sets run out of objects
    while (i < A.size() && j < B.size()) {
        pair<coord_t, coord_t> cur;

        // find the leftmost segment, that haven't been reviewed
        if (A[i].first < B[j].first)
            cur = A[i++];
        else
            cur = B[j++];

        // if it intersects with the last segment in res, combine them (augment the last segment)
        if (res[k].second >= cur.first)
            res[k].second = fmaxl(cur.second, res[k].second);
        else {
        // if not, push it to the result 
            res.push_back(cur);
            k++;
        }
    }

    // do the same, but only to the remainding part of the bigger set

    while (i < A.size()) {
        if (res[k].second >= A[i].first)
            res[k].second = fmaxl(A[i++].second, res[k].second);
        else {
            res.push_back(A[i++]);
            k++;
        }
    }
    while (j < B.size()) {
        if (res[k].second >= B[j].first)
            res[k].second = fmaxl(B[j++].second, res[k].second);
        else {
            res.push_back(B[j++]);
            k++;
        }
    }

    return res;
}

pair<coord_t, coord_t>
intersect_with_circle(Point O, coord_t R, coord_t x) {
    auto res = pair<coord_t, coord_t>(O.y, O.y);

    coord_t h = fabsl(O.x - x);

    if (h - R >= 0.0)
        return res;

    coord_t d = sqrtl((R - h)*(R + h));

    res.first -= d;
    res.second += d;
    return res;
}

vector<pair<coord_t, coord_t>>
intersect_with_smile(Point O, coord_t x) {
    vector<pair<coord_t, coord_t>> res;

    if (fabsl(O.x - x) >= 100.0)
        return res;

    pair<coord_t, coord_t> 
    head = 
        intersect_with_circle(O, 100.0, x);

    pair<coord_t, coord_t> 
    mouth = 
        intersect_with_circle(Point(O.x, O.y - 20.0), 60.0, x);

    mouth.second = O.y - 20.0;

    pair<coord_t, coord_t> eye;

    if (O.x - x > 0.0)
        eye = intersect_with_circle(Point(O.x - 40.0, O.y + 30.0), 30.0, x);
    else
        eye = intersect_with_circle(Point(O.x + 40.0, O.y + 30.0), 30.0, x);

    if (mouth.first != mouth.second) {
        res.push_back(make_pair(head.first, mouth.first));

        if (eye.first != eye.second) {
            res.push_back(make_pair(mouth.second, eye.first));
            res.push_back(make_pair(eye.second, head.second));
        }
        else
            res.push_back(make_pair(mouth.second, head.second));
        
    }
    else {
        if (eye.first != eye.second) {
            res.push_back(make_pair(head.first, eye.first));
            res.push_back(make_pair(eye.second, head.second));
        }
        else
            res.push_back(head);
    }
    return res;
}

coord_t
sum_length(vector<pair<coord_t, coord_t>> A) {
    coord_t res = 0;
    for (auto i = 0LU; i < A.size(); i++) {
        res += A[i].second - A[i].first;
    }
    return res;
}

coord_t 
distance(Point A, Point B) {
    coord_t dx = A.x - B.x;
    coord_t dy = A.y - B.y;
    return sqrtl(dx*dx + dy*dy);
}

coord_t 
slice(Point O1, Point O2, coord_t x) {
    return sum_length(combine_segments(intersect_with_smile(O1, x), intersect_with_smile(O2, x)));
}


coord_t
calculate_space(Point O1, Point O2, coord_t E) {
    if (distance(O1, O2) - 200.0 >= 0.0)
        return 2 * smile_space;
    if ((O1.x - O2.x == 0) && (O1.y - O2.y) == 0)
        return smile_space;

    coord_t left = fminl(O1.x - 100.0, O2.x - 100.0);
    coord_t right = fmaxl(O1.x + 100.0, O2.x + 100.0);

    size_t N = 500000;
    coord_t h = (right - left) / N;

    coord_t res = 0;
    for (auto i = 0LU; i < N; i++) {
        res += calculate_space_recursive(O1, O2, left + i*h, left + (i+1)*h, E);
        // printf("%lu ", i);
    }
    return res;
}

coord_t
calculate_space_recursive(Point O1, Point O2, coord_t a, coord_t b, coord_t E) {

    if (fabsl(b - a) <= E*E)
        return 0;

    coord_t mid = (a + b) / 2;

    coord_t I1 = (b - a)*slice(O1, O2, mid);

    coord_t I2 = (slice(O1, O2, (a + mid) / 2) + slice(O1, O2, (b + mid) / 2)) * (b - a) * 0.5;
    if (fabsl(I1 - I2) - E <= 0.0) {
        return I2;
    }

    coord_t left = calculate_space_recursive(O1, O2, a, mid, E / 2);
    coord_t right = calculate_space_recursive(O1, O2, mid, b, E / 2);
    return left + right;
}

