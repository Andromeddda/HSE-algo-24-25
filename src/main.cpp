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

// random
#include <random>

// math
#include <math.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef typename std::uniform_real_distribution<double> Dist;
typedef typename std::default_random_engine Engine;

using namespace std;

struct Point {
    double x;
    double y;

    Point(double X, double Y) : x(X), y(Y) {}
};

struct Circle {
    Point c;
    double r;

    Circle(double X, double Y, double R) : c(X, Y), r(R) {}
};

double distance(const Point& A, const Point& B);
bool coverage(const Point& A, const vector<Circle>& circles);
size_t Jinn(const vector<Circle>& circles, size_t precision);

const size_t p = 100000;

int 
main(void) 
{
    size_t N;
    cin >> N;

    auto circles = vector<Circle>();

    for (auto i = 0LU; i < N; i++)
    {
        double x, y, r;
        cin >> x >> y >> r;

        circles.push_back(Circle(x, y, r));
    }

    size_t count = Jinn(circles, p);

    cout << ((double)count / (double)p) * 100.0;

    return 0;
}

double 
distance(const Point& A, const Point& B) 
{   
    double dx = A.x - B.x;
    double dy = A.y - B.y;
    return sqrt((dx * dx) + (dy * dy));
}

bool 
coverage(const Point& A, const vector<Circle>& circles)
{
    for (Circle C : circles)
    {
        double R = distance(A, C.c);
        if (C.r - R >= 0.0)
            return true;
    }
    return false;
}

size_t
Jinn (const vector<Circle>& circles, size_t precision) 
{
    size_t res = 0;

    Dist dist;
    Engine re;

    for (; precision != 0; precision--) 
    {
        Point A = Point(dist(re), dist(re));
        if (coverage(A, circles))
            res++;
    }
    return res;
}

