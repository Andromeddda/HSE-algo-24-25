#include <iostream>
#include <fstream>
#include <string>

#include <algorithm>
#include <random>

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <vector>
#include <stack>
#include <list>

#include <utility>
#include <exception>

#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <assert.h>
#include <climits>
#include <cfloat>

using namespace std;

#define INF LDBL_MAX

typedef long double point_t;

struct Point
{
    point_t x, y;
    Point(point_t X, point_t Y) : x(X), y(Y) { }
};

point_t min_radius(const vector<Point>& points);
point_t distance(const Point& A, const Point& B);

int main(void) 
{
    int N;
    cin >> N;

    vector<Point> points;
    for (int i = 0; i < N; i++) {
        point_t x, y;
        cin >> x >> y;

        points.push_back(Point(x, y));
    }

    point_t res = min_radius(points);
    printf("%.2Lf", res);
    return 0;
}


point_t distance(const Point& A, const Point& B)
{
    return sqrtl((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y));
}

point_t min_radius(const vector<Point>& points)
{
    auto distance_to_span   = vector<point_t>(points.size(), INF);
    auto used               = vector<bool>(points.size(), false);
    distance_to_span[0] = 0; // start with first

    // intiialuze unused points
    set<pair<point_t, int>> unused;
    for (int i = 0; i < (int)points.size(); i++)
    {
        unused.insert(make_pair(distance_to_span[i], i));
    }

    while (!unused.empty())
    {
        int current = unused.begin()->second;
        unused.erase(unused.begin());
        used[current] = true;

        // look at vertex
        for (int i = 0; i < (int)points.size(); i++)
        {
            point_t d;
            // if vertex is unused, update its distance to span
            if (!used[i] && distance_to_span[i] > (d = distance(points[current], points[i])))
            {
                unused.erase({distance_to_span[i], i});
                distance_to_span[i] = d;
                unused.insert({distance_to_span[i], i});
            }
        }
    }

    point_t res;
    for (point_t d : distance_to_span)
    {
        res = fmaxl(res, d);
    }
    return res;
}
