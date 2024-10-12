#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

typedef long double real_t;

struct Func {
    real_t a0, a1, a2, a3, a4;

    Func(real_t A, real_t B, real_t C, real_t D, real_t E)
    : a0(A), a1(B), a2(C), a3(D), a4(E) {}

    real_t operator() (real_t x) const  {
        return 1 / (a0 + x*(a1 + x*(a2 + x*(a3 + x*a4))));
    }
};

real_t simpson_approximation(Func f, real_t a, real_t b);
real_t integrate(Func f, real_t a, real_t b, real_t E);


int main(void)
{
    real_t a, b, c, d, e;
    cin >> a >> b >> c >> d >> e;
    Func f(a, b, c, d, e);

    real_t A = -1e9;
    real_t B = 1e9;
    real_t res = 0;
    real_t E = 1e-9;
    int n = 10000000;

    real_t h = (B - A) / n;
    for (int i = 0; i < n; ++i) {
        res += integrate(f, A + i*h, A + (i+1)*h, E);
    }

    printf("%.10Lf\n", res);

    return 0;
}


real_t simpson_approximation(Func f, real_t a, real_t b) {
    return ((b-a)/6) * (f(a) + 4 * f((a+b)/2) + f(b));
}


real_t integrate(Func f, real_t a, real_t b, real_t E) {
    real_t I1 = simpson_approximation(f, a, b);

    real_t I2 = 0;
    real_t h = (b - a) / 5;

    for (auto i = 0LU; i < 5; i++)
        I2 += simpson_approximation(f, a + i*h, a + (i+1)*h);

    if (fabsl(I2 - I1) < E)
        return I2;


    return integrate(f, a, (a + b) / 2, E / 2) + integrate(f, (a + b) / 2, b, E / 2);

}
