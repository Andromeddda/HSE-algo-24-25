#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <random>
#include <math.h>

using namespace std;

#define SPLIT_SIZE (2)
#define INITIAL_SPLIT (100)

typedef signed long long int Int_t;
typedef double Real_t;

Int_t GCD(Int_t a, Int_t b);
Int_t SCD(Int_t a, Int_t b);

class Rational {
private:
    Int_t numerator_;
    Int_t denomenator_;
public:
    Rational(Int_t n, Int_t d);
    inline Rational(Int_t n) : numerator_(n), denomenator_(1) { }

    Rational(const Rational &other) = default;
    Rational(Rational &&other) = default;
    Rational& operator= (const Rational &other) = default;
    Rational& operator= (Rational &&other) = default;

    inline Int_t denom() { return denomenator_; }
    inline Int_t numer() { return numerator_; }
    inline operator Real_t () const { return (Real_t)(numerator_) / (Real_t)(denomenator_); }

    Rational operator- () const;
    Rational operator+ (const Rational &other) const;
    Rational operator- (const Rational &other) const;
    Rational operator* (const Rational &other) const;
    Rational operator/ (const Rational &other) const;

    bool    operator== (const Rational &other) const = default;
    bool    operator!= (const Rational &other) const = default;
    bool    operator>  (const Rational &other) const;
    bool    operator>=  (const Rational &other) const;
    bool    operator<  (const Rational &other) const;
    bool    operator<=  (const Rational &other) const;
};

typedef Real_t coord_t;

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(a) (((a) < coord_t(0)) ? (-(a)) : (a))

struct Point {
    coord_t x;
    coord_t y;
    Point(coord_t X, coord_t Y) : x(X), y(Y) {}
    Point(const Point &other) = default;
    Point(Point &&other) = default;
    Point& operator= (const Point &other) = default;
    Point& operator= (Point &&other) = default;

    inline Point operator+ (const Point &other) const { return Point(x + other.x, y + other.y); }
    inline Point operator- (const Point &other) const { return Point(x - other.x, y - other.y); }
    inline Point operator* (coord_t k) const { return Point(x*k, y*k); }
    inline Point operator/ (coord_t k) const { return Point(x / k, y / k); }
};



struct Chunk {
    Point center;
    coord_t side;

    Chunk() : center(0, 0), side(0) {}
    Chunk(Point C, coord_t S) : center(C), side(S) { }
    inline coord_t space() const { return side*side; }
};

struct Smile {
    Point center;
    Smile(Point p) : center(p) {}
    bool operator() (const Point &p) const;
};

struct TwoSmiles {
    Smile P, Q;
    TwoSmiles(Smile p, Smile q) : P(p), Q(q) {}
    inline bool operator() (const Point &p) const {return P(p) || Q(p); }
};

coord_t distance_square(const Point &A, const Point &B);

coord_t smiles_space(TwoSmiles F,coord_t E);
coord_t smiles_space_recursive(TwoSmiles F, Chunk chunk, coord_t E);

int main(void)
{

    Smile smile = Smile(Point(0, 0));
    TwoSmiles F(smile, smile);

    printf("%.4lf\n", Real_t(smiles_space(F, coord_t(1e-4))));
    return 0;
}


Int_t GCD(Int_t a, Int_t b) {
    if (!a) return b;
    if (!b) return a;
    a = ABS(a);
    b = ABS(b);

    if (a == b) return a;

    Int_t x = MAX(a, b);
    Int_t y = MIN(a, b);

    while (y != 0) {
        Int_t tmp = x % y;
        x = y;
        y = tmp;
    }
    return x;
}

Int_t SCD(Int_t a, Int_t b) {
    if (!a) return 0;
    if (!b) return 0;

    Int_t gcd = GCD(a, b);
    return ABS((a / gcd) * b);
}

Rational::Rational(Int_t n, Int_t d) {
    if (!d)
        throw std::runtime_error("Cannot construct rational with d = 0!\n");

    Int_t sign = ((n < 0) ^ (d < 0)) ? 1 : -1;
    Int_t gcd = GCD(n, d);

    n = ABS(n) / gcd;
    d = ABS(d) / gcd;

    this->numerator_ = n*sign;
    this->denomenator_ = d;
}

Rational Rational::operator- () const {
    return Rational(-numerator_, denomenator_);
}


Rational Rational::operator+ (const Rational &other) const {
    Int_t new_denom = SCD(denomenator_, other.denomenator_);
    Int_t new_num = 
        (new_denom / denomenator_) * numerator_ + (new_denom / other.denomenator_) * other.numerator_;

    return Rational(new_num, new_denom);
}


Rational Rational::operator- (const Rational &other) const {
    return (*this) + (-other);
}

Rational Rational::operator* (const Rational &other) const {
    return Rational(numerator_ * other.numerator_, denomenator_ * other.denomenator_);
}

Rational Rational::operator/ (const Rational &other) const {
    return Rational(numerator_ * other.denomenator_, denomenator_ * other.numerator_);
}

bool    Rational::operator>  (const Rational &other) const {
    return numerator_*other.denomenator_ > other.numerator_*denomenator_;
}

bool    Rational::operator>=  (const Rational &other) const {
    return numerator_*other.denomenator_ >= other.numerator_*denomenator_;
}

bool    Rational::operator<  (const Rational &other) const {
    return numerator_*other.denomenator_ < other.numerator_*denomenator_;
}

bool    Rational::operator<=  (const Rational &other) const {
    return numerator_*other.denomenator_ <= other.numerator_*denomenator_;
}

coord_t distance_square(const Point &A, const Point &B) {
    return (A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y);
}

bool Smile::operator() (const Point &A) const {
    bool in_head = (distance_square(A, center) - coord_t(10000) <= coord_t(0));

    if (!in_head)
        return false;

    Point left_eye_center (center.x - coord_t(40), center.y + coord_t(30));
    Point right_eye_center (center.x + coord_t(40), center.y + coord_t(30));
    Point mouth_center (center.x, center.y - coord_t(20));

    if ((distance_square(A, mouth_center) - coord_t(3600) < coord_t(0)) && (mouth_center.y - A.y > coord_t(0)))
        return false;

    if (distance_square(A, left_eye_center) - coord_t(900) < coord_t(0))
        return false;

    if (distance_square(A, right_eye_center) - coord_t(900) < coord_t(0))
        return false;

    return true;
}


coord_t smiles_space(TwoSmiles F, coord_t E) {
    Chunk chunk (Point(0, 0), coord_t(200));
    coord_t new_side = chunk.side / coord_t(INITIAL_SPLIT);
    Point dx (new_side, 0);
    Point dy (0, new_side);
    Point LL (chunk.center.x - coord_t(INITIAL_SPLIT-1) * (new_side / coord_t(2)), chunk.center.y - coord_t(INITIAL_SPLIT-1) * (new_side / coord_t(2)));

    coord_t res(0);

    for (Int_t i = 0; i < INITIAL_SPLIT; i++) {
        for (Int_t j = 0; j < INITIAL_SPLIT; j++) {
            Point cur = LL + dx*coord_t(i) + dy*coord_t(j);
            res = res + smiles_space_recursive(F, Chunk(cur, new_side), E / coord_t(INITIAL_SPLIT));
        }
    }

    return res;
}


coord_t smiles_space_recursive(TwoSmiles F, Chunk chunk, coord_t E) {
    coord_t I1(0), I2(0);

    if (F(chunk.center))
        I1 = chunk.space();

    // Chunk split[SPLIT_SIZE][SPLIT_SIZE];
    coord_t new_side = chunk.side / coord_t(SPLIT_SIZE);

    Point dx (new_side, 0);
    Point dy (0, new_side);
    Point LL (chunk.center.x - coord_t(SPLIT_SIZE-1) * (new_side / coord_t(2)), chunk.center.y - coord_t(SPLIT_SIZE-1) * (new_side / coord_t(2)));


    for (Int_t i = 0; i < SPLIT_SIZE; i++) {
        for (Int_t j = 0; j < SPLIT_SIZE; j++) {
            Point cur = LL + dx*coord_t(i) + dy*coord_t(j);
            if (F(cur))
                I2 = I2 + new_side*new_side;
        }
    }

    if (ABS(I2 - I1) < E)
        return I2;


    coord_t res(0);

    for (Int_t i = 0; i < SPLIT_SIZE; i++) {
        for (Int_t j = 0; j < SPLIT_SIZE; j++) {
            Point cur = LL + dx*coord_t(i) + dy*coord_t(j);
            res = res + smiles_space_recursive(F, Chunk(cur, new_side), E / (SPLIT_SIZE));
        }
    }

    return res;
}