#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <random>
#include <math.h>

using namespace std;

typedef long double real_t;
typedef std::uniform_real_distribution<real_t> Ud;

std::random_device   gen;
// std::mt19937         gen(rd());

// 2D Point with real_t coordinates
struct Point {
    real_t x;
    real_t y;

    Point(real_t X, real_t Y) : 
    x(X), y(Y) 
    { }
};

struct rectangle_boundary {
    real_t left;
    real_t right;
    real_t up;
    real_t down;

    // Boundary described around the circle (C, R)
    rectangle_boundary(Point C, real_t R) :
        left    (C.x - R),
        right   (C.x + R),
        up      (C.y + R),
        down    (C.y - R) 
    { }

    // Combine two boundaries
    rectangle_boundary(const rectangle_boundary &A, const rectangle_boundary &B) :
        left    (fminl(A.left,   B.left)),
        right   (fmaxl(A.right,  B.right)),
        up      (fmaxl(A.up,     B.up)),
        down    (fminl(A.down,   B.down))
    { }
};

Point   generate_random_point(const rectangle_boundary &b);
real_t  distance(const Point &A, const Point &);
bool    is_in_smile(const Point &A, const Point &C);
real_t boundary_space(const rectangle_boundary& b);

int main(void)
{
    real_t x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;

    Point c1(x1, y1);
    Point c2(x2, y2);

    if (distance(c1, c2) >= 200.0) {
        printf("40212.3859659494\n");
        return 0;
    }

    rectangle_boundary b1(c1, 100.0);
    rectangle_boundary b2(c2, 100.0);
    rectangle_boundary b(b1, b2);

    auto N = 1000000LU;

    auto count = 0LU;
    for (auto i = 0LU; i < N; ++i) {
        Point A = generate_random_point(b);

        if (is_in_smile(A, c1) || is_in_smile(A, c2))
            count++;
    }

    printf("%.5Lf\n", (boundary_space(b) * count) / N);

    return 0;
}

Point generate_random_point(const rectangle_boundary& b) {
    Ud distx(b.left, b.right);
    Ud disty(b.down, b.up);

    return Point(distx(gen), disty(gen));
}

real_t  distance(const Point &A, const Point &B) {
    return sqrtl((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y));
}

bool    is_in_smile(const Point &A, const Point &C) {
    bool in_head = (distance(A, C) - 100.0 <= 0.0);

    if (!in_head)
        return false;

    Point left_eye_center (C.x - 40.0, C.y + 30.0);
    Point right_eye_center (C.x + 40.0, C.y + 30.0);
    Point mouth_center (C.x, C.y - 20.0);

    if ((distance(A, mouth_center) - 60.0 < 0.0) && (mouth_center.y - A.y > 0.0))
        return false;

    if (distance(A, left_eye_center) - 30.0 < 0.0)
        return false;

    if (distance(A, right_eye_center) - 30.0 < 0.0)
        return false;


    return true;
}

real_t boundary_space(const rectangle_boundary& b) {
    return (b.right - b.left) * (b.up - b.down);
}

