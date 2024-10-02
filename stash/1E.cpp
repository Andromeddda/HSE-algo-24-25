#include <cstdio>
#include <cstdlib>
// #include <cstring>
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
#include <set>

// random
#include <random>

// math
#include <math.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(a)    ((a < 0) ? -a : a)

using namespace std;

struct Point {
    int x, y;
    Point(int X, int Y) : x(X), y(Y) {}
};

inline bool operator==(Point A, Point B) { return A.x == B.x && A.y == B.y; }
int rotation(Point A, Point B, Point C);
pair<size_t, size_t> tangent(const vector<Point>& H1, const vector<Point>& H2, bool Rotation);
vector<Point> three_points_hull(vector<Point> points);
vector<Point> combine_hulls(const vector<Point>& H1, const vector<Point>& H2);
vector<Point> convex_hull(const vector<Point>& points);
Point polygon_random(const vector<Point>& P);

int 
main(void) 
{
    vector<Point> s = {Point(0, 0), Point(4, 5), Point(2, 9)};

    for (int i = 0; i < 10; i++) {
        Point r = polygon_random(s);
        cout << "(" << r.x << ", " << r.y << ")\n";
    }
    cout << '\n';
    return 0;
}


/* 
 * Oriented length of vector product AB * BC
 * Return value: 
 *      if A, B, C are collinear, 0 is returned,
 *      if A->B->C is a counterclockwise rotation, positive value is returned,
 *      if A->B->C is a clockwise rotation, negative value is returned. 
*/ 
int rotation(Point A, Point B, Point C) {
    return (B.x - A.x)*(C.y - A.y) - (B.y - A.y)*(C.x - A.x);
}

/*
 * Find outer tangent of two convexes
 * Input:
 *      H1, H2 - non-intersecting convex hulls, sorted in clockwise order
 *      rotation - true if the clockwise rotation of tangent is from H1 to H2, false if from H2 to H1
 * Return value:
 *      pair of indecies p1 and p2 from H1 and H2 respectfully
*/
pair<size_t, size_t> tangent(const vector<Point>& H1, const vector<Point>& H2, bool Rotation) {
    // index of leftmost point in H1
    size_t h1_leftmost_i = 0;
    for (auto i = 1LU; i < H1.size(); i++)
        if (H1[i].x < H1[h1_leftmost_i].x)
            h1_leftmost_i = i;

    // index of leftmost point in H1
    size_t h2_rightmost_i = 0;
    for (auto i = 1LU; i < H2.size(); i++)
        if (H2[i].x > H2[h2_rightmost_i].x)
            h2_rightmost_i = i;

    bool tangent_found;

    // indexes of points that form upper tangent to both hulls
    size_t p1 = h1_leftmost_i;
    size_t p2 = h2_rightmost_i;
    tangent_found = false;

    // P1 and P2 form an upper tangen if and only if 
    // for each point A, rotation(P1, P2, A) > 0
    while (!tangent_found) {
        tangent_found = true;

        size_t p1_next = (p1 + H1.size() + 1) % H1.size();
        while (!(rotation(H1[p1], H2[p2], H1[p1_next]) > 0) ^ Rotation) {
            p1 = p1_next;
            p1_next = (p1 + 1 + H1.size()) % H1.size();
        }

        size_t p1_prev = (p1 + H1.size() - 1) % H1.size();
        while (!(rotation(H1[p1], H2[p2], H1[p1_prev]) > 0) ^ Rotation) {
            p1 = p1_prev;
            p1_prev = (p1 - 1 + H1.size()) % H1.size();
        }

        size_t p2_next = (p2 + H2.size() + 1) % H2.size();
        while (!(rotation(H1[p1], H2[p2], H2[p2_next]) > 0) ^ Rotation) {
            p2 = p2_next;
            p2_next = (p2 + 1 + H2.size()) % H2.size();
            tangent_found = false;
        }

        size_t p2_prev = (p2 + H2.size() - 1) % H2.size();
        while (!(rotation(H1[p1], H2[p2], H2[p2_prev]) > 0) ^ Rotation) {
            p2 = p2_prev;
            p2_prev = (p2 - 1 + H2.size()) % H2.size();
            tangent_found = false;
        }
    }

    return make_pair(p1, p2);
}

vector<Point> three_points_hull(vector<Point> points) {
    assert(points.size() == 3);
    int R = rotation(points[0], points[1], points[2]);
    if (R < 0)
        return points;

    if (R > 0)
        return vector<Point>{points[0], points[2], points[1]};

    size_t left = 0, right = 0;
    for (auto i = 1LU; i < points.size(); i++) {
        if (points[i].x < points[left].x)
            left = i;
        if (points[i].x > points[right].x)
            right = i;
    }
    return vector<Point>{points[left], points[right]};

}

/*
 * Create a convex hull of combination of two convex hulls
 * Input: two non-intersecting convex hulls sorted clockwise
 * Return value: convex hull sorted clockwise
*/
vector<Point> combine_hulls(const vector<Point>& H1, const vector<Point>& H2) {
    auto res = vector<Point>();

    pair<size_t, size_t> upper = tangent(H1, H2, true);
    pair<size_t, size_t> lower = tangent(H1, H2, false);

    size_t i = lower.first;
    while (i != upper.first) {
        res.push_back(H1[i]);
        i = (i + 1) % H1.size();
    }
    res.push_back(H1[i]);

    i = upper.second;
    while (i != lower.second) {
        res.push_back(H2[i]);
        i = (i + 1) % H2.size();
    }
    res.push_back(H2[i]);

    return res;
}

/*
 * Return convex hull sorted clockwise
 * Complexity O(N)
 */
vector<Point> convex_hull(const vector<Point>& points) {
    if (points.size() < 3)
        return points;
    if (points.size() == 3)
        return three_points_hull(points);

    // split into two non-intersecting parts

    // find two points that are guarrantied at the hull
    size_t left_low = 0, up_right = 0;
    for (auto i = 1LU; i < points.size(); i++) {
        if (points[i].x < points[left_low].x || 
            (points[i].x == points[left_low].x && points[i].y < points[left_low].y))
            left_low = i;

        if (points[i].y > points[up_right].y || 
            (points[i].y == points[up_right].y && points[i].x > points[up_right].x))
            up_right = i;
    }
    auto P = vector<Point>();
    auto Q = vector<Point>();

    size_t i = 0;
    while (i < points.size()) {
        if (i == left_low || i == up_right) {
            i++;
            continue;
        }

        if (rotation(points[left_low], points[up_right], points[i]) >= 0)
            P.push_back(points[i]);
        else
            Q.push_back(points[i]);
        i++;
    }

    if (P.size() < Q.size()) {
        P.push_back(points[left_low]);
        P.push_back(points[up_right]);
    }
    else {
        Q.push_back(points[left_low]);
        Q.push_back(points[up_right]);
    }
    auto pconv = convex_hull(P);
    auto qconv = convex_hull(Q);
    return combine_hulls(pconv, qconv);
}

Point polygon_random(const vector<Point>& P) {
    // find square bounds of polygon
    int l = 0, r = 0, u = 0, d = 0;
    for (auto i = 1LU; i < P.size(); i++) {
        if (P[i].x < l)
            l = P[i].x;
        if (P[i].x > r)
            r = P[i].x;
        if (P[i].y < d)
            d = P[i].y;
        if (P[i].y > u)
            u = P[i].y;
    }

    if (r == l || u == d)
        return P[0];

    // generate random in square until it's in P
    while (true) {
        int x = (std::rand() % (r - l)) + l;
        int y = (std::rand() % (u - d)) + d;
        Point res = Point(x, y);

        auto Q = vector<Point>(P);
        Q.push_back(res);

        if (convex_hull(P) == convex_hull(Q))
            return res;
    }

    // we should not be here
    return P[0];
}
