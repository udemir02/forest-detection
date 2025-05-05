#if !defined(RANDOM_HEADER_H)
#define RANDOM_HEADER_H

#include <assert.h>
#include <math.h>
#include <limits.h>

#define RMOD INT_MAX
#define RMULT 48271
#define RDEFAULT 123456789

static int rseed = RDEFAULT;

double random(void) {
    long Q = RMOD / RMULT;
    long R = RMOD % RMULT;
    long t = RMULT * (rseed % Q) - R * (rseed / Q);

    if (t > 0)
        rseed = t;
    else
        rseed = t + RMOD;

    return (double)rseed / RMOD;
}

void randomize(int num) {
    for (int i = 0; i < num; i++) random();
}

double uniform(double a, double b) {
    assert(a < b);

    double result = a + (b - a) * random();
    return result;
}

int equilikely(int a, int b) {
    assert(a < b);

    int result = a + (b - a + 1) * random();
    return result;
}

double exponential(double m) {
    double result = -m * log(1.0 - random());
    return result;
}

int bernoulli(double p) {
    assert((p > 0.0) && (p < 1.0));

    int result = (random() < (1.0 - p)) ? 0 : 1;
    return result;
}

int geometric(double m) {
    assert((m > 0.0) && (m < 1.0));

    int result = (int)(log(1.0 - random()) / log(m));
    return result;
}

int binomial(int n, double p) {
    assert(n > 0);

    int result = 0;

    for (int i = 0; i < n; i++)
        result += bernoulli(p);

    return result;
}

int pascal(int n, double p) {
    assert(n > 0);

    int result = 0;

    for (int i = 0; i < n; i++)
        result += geometric(p);

    return result;
}

double erlang(int n, double b) {
    assert(n > 0);

    double result = 0.0;

    for (int i = 0; i < n; i++)
        result += exponential(b);

    return result;
}

int poisson(double m) {
    assert(m > 0.0);

    int result = 0;
    double t = 0.0;

    while (t < m) {
        t += exponential(1.0);
        result++;
    }

    result--;

    return result;
}

double normal(double mean, double std) {
    double x, tmp, r1, r2, r3;

    do {
        r1 = random();
        r2 = random();
        x = -log(r1);
        tmp = exp(-((x - 1) * (x - 1)) / 2);
    } while (r2 > tmp);

    r3 = random();

    double result = (r1 > 0.5) ? (mean + std * x) : (mean - std * x);

    return result;
}

double lognormal(double mean, double std) {
    double result = exp(mean + std * normal(0, 1));
}

#endif
