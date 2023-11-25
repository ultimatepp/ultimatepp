#include "Geom.h"

namespace Upp {

// Based on:
// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2023
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
// https://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// Version: 6.0.2023.08.08

double LineSegmentsDistance(const Pointf3& P0, const Pointf3& P1, const Pointf3& Q0, const Pointf3& Q1)
{
    Pointf3 P1mP0 = P1 - P0;
    Pointf3 Q1mQ0 = Q1 - Q0;
    Pointf3 P0mQ0 = P0 - Q0;
    double a = ScalarProduct(P1mP0, P1mP0);
    double b = ScalarProduct(P1mP0, Q1mQ0);
    double c = ScalarProduct(Q1mQ0, Q1mQ0);
    double d = ScalarProduct(P1mP0, P0mQ0);
    double e = ScalarProduct(Q1mQ0, P0mQ0);
    double det = a * c - b * b;
    double s, t, nd, bmd, bte, ctd, bpe, ate, btd;

    double const zero = static_cast<double>(0);
    double const one = static_cast<double>(1);
    if (det > zero)
    {
        bte = b * e;
        ctd = c * d;
        if (bte <= ctd)  // s <= 0
        {
            s = zero;
            if (e <= zero)  // t <= 0
            {
                // region 6
                t = zero;
                nd = -d;
                if (nd >= a)
                {
                    s = one;
                }
                else if (nd > zero)
                {
                    s = nd / a;
                }
                // else: s is already zero
            }
            else if (e < c)  // 0 < t < 1
            {
                // region 5
                t = e / c;
            }
            else  // t >= 1
            {
                // region 4
                t = one;
                bmd = b - d;
                if (bmd >= a)
                {
                    s = one;
                }
                else if (bmd > zero)
                {
                    s = bmd / a;
                }
                // else:  s is already zero
            }
        }
        else  // s > 0
        {
            s = bte - ctd;
            if (s >= det)  // s >= 1
            {
                // s = 1
                s = one;
                bpe = b + e;
                if (bpe <= zero)  // t <= 0
                {
                    // region 8
                    t = zero;
                    nd = -d;
                    if (nd <= zero)
                    {
                        s = zero;
                    }
                    else if (nd < a)
                    {
                        s = nd / a;
                    }
                    // else: s is already one
                }
                else if (bpe < c)  // 0 < t < 1
                {
                    // region 1
                    t = bpe / c;
                }
                else  // t >= 1
                {
                    // region 2
                    t = one;
                    bmd = b - d;
                    if (bmd <= zero)
                    {
                        s = zero;
                    }
                    else if (bmd < a)
                    {
                        s = bmd / a;
                    }
                    // else:  s is already one
                }
            }
            else  // 0 < s < 1
            {
                ate = a * e;
                btd = b * d;
                if (ate <= btd)  // t <= 0
                {
                    // region 7
                    t = zero;
                    nd = -d;
                    if (nd <= zero)
                    {
                        s = zero;
                    }
                    else if (nd >= a)
                    {
                        s = one;
                    }
                    else
                    {
                        s = nd / a;
                    }
                }
                else  // t > 0
                {
                    t = ate - btd;
                    if (t >= det)  // t >= 1
                    {
                        // region 3
                        t = one;
                        bmd = b - d;
                        if (bmd <= zero)
                        {
                            s = zero;
                        }
                        else if (bmd >= a)
                        {
                            s = one;
                        }
                        else
                        {
                            s = bmd / a;
                        }
                    }
                    else  // 0 < t < 1
                    {
                        // region 0
                        s /= det;
                        t /= det;
                    }
                }
            }
        }
    }
    else
    {
        // The segments are parallel. The quadratic factors to
        //   R(s,t) = a*(s-(b/a)*t)^2 + 2*d*(s - (b/a)*t) + f
        // where a*c = b^2, e = b*d/a, f = |P0-Q0|^2, and b is not
        // zero. R is constant along lines of the form s-(b/a)*t = k
        // and its occurs on the line a*s - b*t + d = 0. This line
        // must intersect both the s-axis and the t-axis because 'a'
        // and 'b' are not zero. Because of parallelism, the line is
        // also represented by -b*s + c*t - e = 0.
        //
        // The code determines an edge of the domain [0,1]^2 that
        // intersects the minimum line, or if none of the edges
        // intersect, it determines the closest corner to the minimum
        // line. The conditionals are designed to test first for
        // intersection with the t-axis (s = 0) using
        // -b*s + c*t - e = 0 and then with the s-axis (t = 0) using
        // a*s - b*t + d = 0.

        // When s = 0, solve c*t - e = 0 (t = e/c).
        if (e <= zero)  // t <= 0
        {
            // Now solve a*s - b*t + d = 0 for t = 0 (s = -d/a).
            t = zero;
            nd = -d;
            if (nd <= zero)  // s <= 0
            {
                // region 6
                s = zero;
            }
            else if (nd >= a)  // s >= 1
            {
                // region 8
                s = one;
            }
            else  // 0 < s < 1
            {
                // region 7
                s = nd / a;
            }
        }
        else if (e >= c)  // t >= 1
        {
            // Now solve a*s - b*t + d = 0 for t = 1 (s = (b-d)/a).
            t = one;
            bmd = b - d;
            if (bmd <= zero)  // s <= 0
            {
                // region 4
                s = zero;
            }
            else if (bmd >= a)  // s >= 1
            {
                // region 2
                s = one;
            }
            else  // 0 < s < 1
            {
                // region 3
                s = bmd / a;
            }
        }
        else  // 0 < t < 1
        {
            // The point (0,e/c) is on the line and domain, so we have
            // one point at which R is a minimum.
            s = zero;
            t = e / c;
        }
    }

    Pointf3 p1 = P0 + s * P1mP0;
    Pointf3 p2 = Q0 + t * Q1mQ0;
    return Distance(p1, p2);
}

};