#include "Equation.h"
unsigned Equation::root4f(double a, double b, double c, double d, double e, double* x) const {
    if (a == 0) {
        if (b == 0) {
            if (c == 0) {
                if (d == 0) {
                    return 0;
                }
                else {
                    *x = -e / d;
                    return 1;
                }
            }
            else
                return root2(d / c, e / c, x);
        }
        else
            return root3(c / b, d / b, e / b, x);
    }
    else
        return root4(b / a, c / a, d / a, e / a, x);
}
unsigned Equation::root4s(double p, double q, double r, double* x) const
{
    if (r == 0) // это условие добавлено 21.10.2008
    {
        *x = 0;
        return 1 + root3s(p, q, &x[1]);
    }
    if (q == 0)
    {
    m1:     
        double t[2];
        unsigned n = root2(p, r, t);
        if (n == 0) return 0;
        if (t[0] >= 0)
        {
            x[0] = sqrt(t[0]);
            x[1] = -x[0];
            n = 2;
        }
        else
        {
            n = 0;
        }
        if (t[1] >= 0)
        {
            x[n] = sqrt(t[1]);
            x[n + 1] = -x[n];
            n += 2;
        }
        return n;
    }
    unsigned n = root3(p + p, p * p - 4. * r, -q * q, x);
    double a = *x;
    if (n == 3)
    {
        if (a < x[1]) a = x[1];
        if (a < x[2]) a = x[2];
    }
    if (a <= 0) goto m1; // этот переход сделан 09.12.2011
    p += a;
    a = sqrt(a);
    const double b = q / a;
    n = root2(a, 0.5 * (p - b), x);
    n += root2(-a, 0.5 * (p + b), x + n);
    return n;
}

// x^4 + a * x^3 + b * x^2 + c * x + d = 0

unsigned Equation::root4(double a, double b, double c, double d, double* x) const
{
    if (a == 0)
    {
        return root4s(b, c, d, x);
    }
    if (d == 0)
    {
        *x = 0;
        return 1 + root3(a, b, c, x + 1);
    }
    const double e = a / 4;
    const double h = e * e;
    const double p = -6 * h + b;
    const double q = 8 * h * e - 2 * b * e + c;
    const double r = -3 * h * h + b * h - c * e + d;
    const unsigned n = root4s(p, q, r, x);
    for (unsigned i = 0; i < n; ++i) x[i] -= e;
    return n;
}

unsigned Equation::root2(double a, double b, double* x)const
{
    if (b == 0)
    {
        x[0] = 0.;
        x[1] = -a;
    }
    else
    {
        a *= -0.5;
        double d = a * a - b;
        if (d < 0) return 0;
        d = sqrt(d);
        x[1] = a > 0 ? a + d : a - d;
        x[0] = b / x[1];
    }
    return 2;
}
double cubic(double a, double b)
{
    double s = 1.;
    while (b + a > -1.)
    {
        a *= 4.;
        b *= 8.;
        s *= 0.5;
    }
    while (b + a + a < -8.)
    {
        a *= 0.25;
        b *= 0.125;
        s *= 2.;
    }
    double x = 1.5;
    x -= (x * (x * x + a) + b) / (3. * x * x + a);
    x -= (x * (x * x + a) + b) / (3. * x * x + a);
    x -= (x * (x * x + a) + b) / (3. * x * x + a);
    x -= (x * (x * x + a) + b) / (3. * x * x + a);
    x -= (x * (x * x + a) + b) / (3. * x * x + a);
    x -= (x * (x * x + a) + b) / (3. * x * x + a);
    x -= (x * (x * x + a) + b) / (3. * x * x + a);
    x -= (x * (x * x + a) + b) / (3. * x * x + a);
    x -= (x * (x * x + a) + b) / (3. * x * x + a);
    return x * s;
}
unsigned Equation::root3s(double p, double q, double* x)const
{
    if (q < 0)
    {
        *x = cubic(p, q);
    }
    else
        if (q > 0)
        {
            *x = -cubic(p, -q);
        }
        else
        {
            *x = 0;
        }
    return 1 + root2(*x, p + (*x) * (*x), &x[1]);
}
unsigned Equation::root3(double a, double b, double c, double* x)const
{
    if (c == 0)
    {
        *x = 0;
    }
    else
    {
        const double a3 = a / 3.;
        const double p = b - a3 * a;
        const double q = c - (a3 * a3 + p) * a3;
        if (q < 0)
        {
            *x = cubic(p, q);
        }
        else
            if (q > 0)
            {
                *x = -cubic(p, -q);
            }
            else
            {
                *x = 0;
            }
        *x -= a3;
        const double t = *x * (*x * 3. + a * 2.) + b;
        if (fabs(t) > 1e-3)
        {
            *x -= (*x * (*x * (*x + a) + b) + c) / t;
        }
        a += *x;
        b += *x * a;
    }
    return 1 + root2(a, b, x + 1);
}

// 