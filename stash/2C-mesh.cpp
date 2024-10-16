#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <random>
#include <math.h>
#include <fstream>
#include <assert.h>

using namespace std;

#define LONG
#ifndef LONG
typedef double coord_t;
#define acosl acos
#define fabsl fabs
#define fmaxl fmax
#define fminl fmin
#define sqrtl sqrt
#else
typedef long double coord_t;
#endif 

const coord_t pi = 3.141592653589793238462643383279;

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

vector<coord_t> xs;
vector<coord_t> ys;

struct Point {
    coord_t x;
    coord_t y;
    Point(coord_t X, coord_t Y) : x(X), y(Y) {}
    Point(const Point &other) = default;
    Point(Point &&other) = default;
    Point& operator= (const Point &other) = default;
    Point& operator= (Point &&other) = default;
};

coord_t 
distance(Point A, Point B);

// Sector of the circle in counterclockwise rotation
struct Sector {
    Point center; // center of the circle
    coord_t radius; // radius of the circle
    coord_t start; // the angle between starting chord of sector and Ox (counterclockwise)
    coord_t angle; // the angle between ending chord and starting chord (counterclockwise)

    Sector(Point O, coord_t R, coord_t S, coord_t A) :
        center(O), radius(R), start(S), angle(A)
    {}

    Sector(coord_t x, coord_t y, coord_t R, coord_t S, coord_t A) :
        center(x, y), radius(R), start(S), angle(A)
    {}

    /*
     *  Return true if point A is inside of the sector, false otherwise
     */
    bool operator() (Point A) const;
};

void create_py(const string& filename);
vector<vector<Sector>> scan_sectors(const string& filename);
bool in_union(vector<Sector> sectors, Point A);
coord_t union_space(const vector<Sector>& sectors, coord_t E);
coord_t union_space_recursive(const vector<Sector>& sectors, Point DL, Point UR, coord_t E);

#define DEBUG

int main(void)
{
    vector<vector<Sector>> sectors = scan_sectors("input.txt");

    for (auto &sec : sectors) {
        cout << union_space(sec, 0.000001L) << '\n';
    }

    #ifdef DEBUG
    create_py("plt.py");
    #endif

    return 0;
}

void create_py(const string& filename) {
    ofstream myfile; 
    myfile.open(filename);

    myfile << "import matplotlib as mpl\nimport matplotlib.pyplot as plt\n\nfig = plt.figure()\nax = fig.gca()\n";

    myfile << "ax.plot([";
    for (auto i = 0LU; i < xs.size(); i++) {
        myfile << xs[i];
        if (i != xs.size() - 1)
            myfile << ", ";
    }
    myfile << "], [";

    for (auto i = 0LU; i < ys.size(); i++) {
        myfile << ys[i];
        if (i != ys.size() - 1)
            myfile << ", ";
    }
    myfile << "], \'.\')\n";

    myfile << "plt.show()\n";

    myfile.close();
}


coord_t 
distance(Point A, Point B) {
    coord_t dx = A.x - B.x;
    coord_t dy = A.y - B.y;
    return sqrtl(dx*dx + dy*dy);
}

bool Sector::operator() (Point A) const {
    coord_t d = distance(A, center);

    if (d - radius >= 0.0L)
        return false;
    if (d == 0.0L)
        return true;

    coord_t dx = A.x - center.x;
    coord_t dy = A.y - center.y;

    coord_t cosinus = dx / d;
    coord_t sinus = dy / d;

    coord_t T = acosl(cosinus);
    if (sinus < 0.0L)
        T += pi;

    if (((angle - T + start) >= 0.0L) && ((T - start) >= 0.0L))
        return true;

    T += 2.0L * pi;

    return ((angle - T + start) >= 0.0L) && ((T - start) >= 0.0L);
}

vector<vector<Sector>> scan_sectors(const string& filename) {
    ifstream myfile; 
    myfile.open(filename);

    int N;
    coord_t x, y, r, s, a;
    vector<vector<Sector>> res;

    while (myfile >> N) {
        res.push_back(vector<Sector>());

        for (int i = 0; i < N; i++) {
            myfile >> x >> y >> r >> s >> a;
            res[res.size() - 1].push_back(Sector(x, y, r, s, a));
        }
    }

    myfile.close();
    return res;
}

bool in_union(vector<Sector> sectors, Point A) {
    for (Sector s : sectors) {
        if (s(A)) {
            return true;
        }
    }
    return false;
}

coord_t union_space(const vector<Sector>& sectors, coord_t E) {
    coord_t res = 0.0L;
    coord_t a = -10.0L;
    coord_t b = 5.0L;

    int N = 5000;
    coord_t h = (b - a) / N;

    coord_t cur = 0.0L;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cur = union_space_recursive(sectors, Point(a + i*h, a + j*h), Point(a + (i+1)*h, a + (j+1)*h), E);
            res += cur;
        }
    }

    return res;
}

coord_t union_space_recursive(const vector<Sector>& sectors, Point DL, Point UR, coord_t E) {
    coord_t dx = UR.x - DL.x;
    coord_t dy = UR.y - DL.y;

    coord_t midx = (DL.x + UR.x)*0.5L;
    coord_t midy = (DL.y + UR.y)*0.5L;

    Point M (midx, midy);
    Point UL (DL.x, UR.y);
    Point DR (UR.x, DL.y);

    Point Lmid (DL.x, midy);
    Point Rmid (UR.x, midy);
    Point Dmid (midx, DL.y);
    Point Umid (midx, UR.y);

    bool m, um, lm, rm, dm;
    bool ul, ur, dl, dr;

    ul = in_union(sectors, UL);
    ur = in_union(sectors, UR);
    dl = in_union(sectors, DL);
    dr = in_union(sectors, DR);

    m  = in_union(sectors, M);
    um = in_union(sectors, Umid);
    lm = in_union(sectors, Lmid);
    rm = in_union(sectors, Rmid);
    dm = in_union(sectors, Dmid);

    coord_t I1 = 0.0L, I2 = 0.0L;
    coord_t q = dx*dy*0.25L;

    if (ul && dr && dl && ur)
        I1 = dx*dy;

    if (dl && m && lm && dm)
        I2 += q;
    if (lm && um && ul && m)
        I2 += q;
    if (m && ur && um && rm)
        I2 += q;
    if (dm && rm && m && dr)
        I2 += q;

    if (fabsl(I1 - I2) - E <= 0.0L)
        return I2;

    #ifdef DEBUG
    xs.push_back(midx);
    ys.push_back(midy);
    #endif


    E = E*0.5L;
    coord_t q1 = union_space_recursive(sectors, DL, M, E);
    coord_t q2 = union_space_recursive(sectors, M, UR, E); 
    coord_t q3 = union_space_recursive(sectors, Lmid, Umid, E); 
    coord_t q4 = union_space_recursive(sectors, Dmid, Rmid, E);

    return q1 + q2 + q3 + q4;

}
