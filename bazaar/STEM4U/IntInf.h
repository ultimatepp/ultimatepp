/*
 * InfInt - Arbitrary-Precision Integer Arithmetic Library
 * Copyright (C) 2013 Sercan Tutar
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
 
#ifndef _STEM4U_IntInf_h_
#define _STEM4U_IntInf_h_

namespace Upp {

#ifdef _MSC_VER
#define LONG_LONG_MIN LLONG_MIN
#define LONG_LONG_MAX LLONG_MAX
#define ULONG_LONG_MAX ULLONG_MAX
#endif

typedef int ELEM_TYPE;
typedef long long PRODUCT_TYPE;
static const ELEM_TYPE BASE = 1000000000;
static const ELEM_TYPE UPPER_BOUND = 999999999;
static const ELEM_TYPE DIGIT_COUNT = 9;
static const int powersOfTen[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };

template <class T>
struct cdiv_t {
    T quot{};
    T rem{};
};

template <class T>
constexpr cdiv_t<T> cdiv(T num, T denom) {
    cdiv_t<T> result;
    result.quot = num / denom;
    result.rem = num - denom * result.quot;
    return result;
}

class intInf {
public:
    /* constructors */
    intInf();
    intInf(const char* c);
    intInf(const String& s);
    template <typename T>
	intInf(T l);
    intInf(unsigned int l);
    intInf(unsigned long l);
    intInf(unsigned long long l);
    intInf(const intInf& l);

    /* assignment operators */
    const intInf& operator=(const char* c);
    const intInf& operator=(const String& s);
    const intInf& operator=(int l);
    const intInf& operator=(long l);
    const intInf& operator=(long long l);
    const intInf& operator=(unsigned int l);
    const intInf& operator=(unsigned long l);
    const intInf& operator=(unsigned long long l);
    const intInf& operator=(const intInf& l);

    /* unary increment/decrement operators */
    const intInf& operator++();
    const intInf& operator--();
    intInf operator++(int);
    intInf operator--(int);

    /* operational assignments */
    const intInf& operator+=(const intInf& right);
    const intInf& operator-=(const intInf& right);
    const intInf& operator*=(const intInf& right);
    const intInf& operator/=(const intInf& right);
    const intInf& operator%=(const intInf& right);
    const intInf& operator*=(ELEM_TYPE right);

    /* operations */
    intInf operator-() const;
    intInf operator+(const intInf& right) const;
    intInf operator-(const intInf& right) const;
    intInf operator*(const intInf& right) const;
    intInf operator/(const intInf& right) const; 
    intInf operator%(const intInf& right) const; 
    intInf operator*(ELEM_TYPE right) const;

	template <typename T>
	intInf operator+(T right) const {return *this + intInf(right);}
	template <typename T>
	intInf operator-(T right) const {return *this - intInf(right);}
	template <typename T>
	intInf operator/(T right) const {return *this / intInf(right);}
			
    /* relational operations */
    bool operator==(const intInf& right) const;
    bool operator!=(const intInf& right) const;
    bool operator<(const intInf& right) const;
    bool operator<=(const intInf& right) const;
    bool operator>(const intInf& right) const;
    bool operator>=(const intInf& right) const;

	template <typename T>
	bool operator<(T right) const {
		return *this < intInf(right);
	}
	template <typename T>
	bool operator==(T right) const {
		return *this == intInf(right);
	}
	template <typename T>
	bool operator>=(T right) const {
		return *this >= intInf(right);
	}
			
    /* integer square root */
    intInf intSqrt() const; 

    /* digit operations */
    char digitAt(int i) const; 
    int numberOfDigits() const;

    /* size in bytes */
    int size() const;

    /* string conversion */
    String ToString() const;

    /* conversion to primitive types */
    int toInt() const; 
    long toLong() const; 
    long long toLongLong() const; 
    unsigned int toUnsignedInt() const; 
    unsigned long toUnsignedLong() const; 
    unsigned long long toUnsignedLongLong() const; 

	operator int() 					{return toInt();}
	operator long() 				{return toLong();}
	operator long long()			{return toLongLong();}
	operator unsigned int() 		{return toUnsignedInt();}
	operator unsigned long() 		{return toUnsignedLong();}
	operator unsigned long long() 	{return toUnsignedLongLong();}
	
private:
    static ELEM_TYPE dInR(const intInf& R, const intInf& D);
    static void multiplyByDigit(ELEM_TYPE factor, Vector<ELEM_TYPE>& val);

    void correct(bool justCheckLeadingZeros = false, bool hasValidSign = false);
    void fromString(const String& s);
    void optimizeSqrtSearchBounds(intInf& lo, intInf& hi) const;
    void truncateToBase();
    bool equalizeSigns();
    void removeLeadingZeros();

    Vector<ELEM_TYPE> val; // number with base FACTOR
    bool pos; // true if number is positive
};

inline intInf::intInf() : pos(true) {
    val.push_back((ELEM_TYPE) 0);
}

inline intInf::intInf(const char* c) {
    fromString(c);
}

inline intInf::intInf(const String& s) {
    fromString(s);
}

template <class T>
inline intInf::intInf(T l) : pos(l >= 0) {
    bool subtractOne = false;
    if (l == std::numeric_limits<T>::min()) {
        subtractOne = true;
        ++l;
    }
    if (!pos)
        l = -l;
    
    do {
        auto dt = cdiv<T>(l, BASE);
        val.push_back((ELEM_TYPE) dt.rem);
        l = dt.quot;
    } while (l > 0);

    if (subtractOne)
        --*this;
}

inline intInf::intInf(unsigned int l) : pos(true) {
    do {
        val.push_back((ELEM_TYPE) (l % BASE));
        l = l / BASE;
    } while (l > 0);
}

inline intInf::intInf(unsigned long l) : pos(true) {
    do {
        val.push_back((ELEM_TYPE) (l % BASE));
        l = l / BASE;
    } while (l > 0);
}

inline intInf::intInf(unsigned long long l) : pos(true) {
    do {
        val.push_back((ELEM_TYPE) (l % BASE));
        l = l / BASE;
    } while (l > 0);
}

inline intInf::intInf(const intInf& l) : pos(l.pos), val(clone(l.val)) {}

inline const intInf& intInf::operator=(const char* c) {
    fromString(c);
    return *this;
}

inline const intInf& intInf::operator=(const String& s) {
    fromString(s);
    return *this;
}

inline const intInf& intInf::operator=(int l) {
    bool subtractOne = false;
    if (l == INT_MIN) {
        subtractOne = true;
        ++l;
    }
    pos = l >= 0;
    val.clear();
    if (!pos)
        l = -l;
    do {
        auto dt = cdiv<int>(l, BASE);
        val.push_back((ELEM_TYPE) dt.rem);
        l = dt.quot;
    } while (l > 0);

    return subtractOne ? --*this : *this;
}

inline const intInf& intInf::operator=(long l) {
    bool subtractOne = false;
    if (l == LONG_MIN) {
        subtractOne = true;
        ++l;
    }
    pos = l >= 0;
    val.clear();
    if (!pos)
        l = -l;
    do {
        auto dt = cdiv<long>(l, BASE);
        val.push_back((ELEM_TYPE) dt.rem);
        l = dt.quot;
    } while (l > 0);

    return subtractOne ? --*this : *this;
}

inline const intInf& intInf::operator=(long long l) {
    bool subtractOne = false;
    if (l == LONG_LONG_MIN) {
        subtractOne = true;
        ++l;
    }
    pos = l >= 0;
    val.clear();
    if (!pos)
        l = -l;
    do {
        auto dt = cdiv<long long>(l, BASE);
        val.push_back((ELEM_TYPE) dt.rem);
        l = dt.quot;
    } while (l > 0);

    return subtractOne ? --*this : *this;
}

inline const intInf& intInf::operator=(unsigned int l) {
    pos = true;
    val.clear();
    do {
        val.push_back((ELEM_TYPE) (l % BASE));
        l = l / BASE;
    } while (l > 0);
    return *this;
}

inline const intInf& intInf::operator=(unsigned long l) {
    pos = true;
    val.clear();
    do {
        val.push_back((ELEM_TYPE) (l % BASE));
        l = l / BASE;
    } while (l > 0);
    return *this;
}

inline const intInf& intInf::operator=(unsigned long long l) {
    pos = true;
    val.clear();
    do {
        val.push_back((ELEM_TYPE) (l % BASE));
        l = l / BASE;
    } while (l > 0);
    return *this;
}

inline const intInf& intInf::operator=(const intInf& l) {
    pos = l.pos;
    val = clone(l.val);
    return *this;
}

inline const intInf& intInf::operator++() {
    val[0] += (pos ? 1 : -1);
    this->correct(false, true);
    return *this;
}

inline const intInf& intInf::operator--() {
    val[0] -= (pos ? 1 : -1);
    this->correct(false, true);
    return *this;
}

inline intInf intInf::operator++(int) {
    intInf result = *this;
    val[0] += (pos ? 1 : -1);
    this->correct(false, true);
    return result;
}

inline intInf intInf::operator--(int) {
    intInf result = *this;
    val[0] -= (pos ? 1 : -1);
    this->correct(false, true);
    return result;
}

inline const intInf& intInf::operator+=(const intInf& right) {
    if (right.val.GetCount() > val.GetCount())
        val.SetCount(right.val.GetCount(), 0);
    for (int i = 0; i < val.GetCount(); ++i)
        val[i] = (pos ? val[i] : -val[i]) + (i < right.val.GetCount() ? (right.pos ? right.val[i] : -right.val[i]) : 0);
    correct();
    return *this;
}

inline const intInf& intInf::operator-=(const intInf& right) {
    if (right.val.GetCount() > val.GetCount())
        val.SetCount(right.val.GetCount(), 0);
    for (int i = 0; i < val.GetCount(); ++i)
        val[i] = (pos ? val[i] : -val[i]) - (i < right.val.GetCount() ? (right.pos ? right.val[i] : -right.val[i]) : 0);
    correct();
    return *this;
}

inline const intInf& intInf::operator*=(const intInf& right) {
    *this = *this * right;
    return *this;
}

inline const intInf& intInf::operator/=(const intInf& right) {
    if (right == 0)
        throw Exc("division by zero");

    intInf R, D = (right.pos ? right : -right), N = (pos ? *this : -*this);
    bool oldpos = pos;
    val.Set(0, 0, val.GetCount());
    for (int i = (int) N.val.GetCount() - 1; i >= 0; --i) {
        R.val.Insert(0, N.val[i]);
        R.correct(true);
        ELEM_TYPE cnt = dInR(R, D);
        R -= D * cnt;
        val[i] += cnt;
    }
    correct();
    pos = (val.GetCount() == 1 && val[0] == 0) ? true : (oldpos == right.pos);
    return *this;
}

inline const intInf& intInf::operator%=(const intInf& right) {
    *this = *this % right;
    return *this;
}

inline const intInf& intInf::operator*=(ELEM_TYPE right) {
    ELEM_TYPE factor = right < 0 ? -right : right;
    bool oldpos = pos;
    multiplyByDigit(factor, val);
    correct();
    pos = (val.GetCount() == 1 && val[0] == 0) ? true : (oldpos == (right >= 0));
    return *this;
}

inline intInf intInf::operator-() const {
    intInf result = *this;
    result.pos = !pos;
    return result;
}

inline intInf intInf::operator+(const intInf& right) const {
    intInf result;
    result.val.SetCount(val.GetCount() > right.val.GetCount() ? val.GetCount() : right.val.GetCount(), 0);
    for (int i = 0; i < val.GetCount() || i < right.val.GetCount(); ++i)
        result.val[i] = (i < val.GetCount() ? (pos ? val[i] : -val[i]) : 0) + (i < right.val.GetCount() ? (right.pos ? right.val[i] : -right.val[i]) : 0);
    result.correct();
    return result;
}

inline intInf intInf::operator-(const intInf& right) const {
    intInf result;
    result.val.SetCount(val.GetCount() > right.val.GetCount() ? val.GetCount() : right.val.GetCount(), 0);
    for (int i = 0; i < val.GetCount() || i < right.val.GetCount(); ++i)
        result.val[i] = (i < val.GetCount() ? (pos ? val[i] : -val[i]) : 0) - (i < right.val.GetCount() ? (right.pos ? right.val[i] : -right.val[i]) : 0);
    result.correct();
    return result;
}

inline intInf intInf::operator*(const intInf& right) const {
    intInf result;
    result.val.SetCount(val.GetCount() + right.val.GetCount(), 0);
    PRODUCT_TYPE carry = 0;
    int digit = 0;
    for (;; ++digit) {
        auto dt = cdiv<long long>(carry, BASE);
        carry = dt.quot;
        result.val[digit] = (ELEM_TYPE) dt.rem;

        bool found = false;
        for (int i = digit < right.val.GetCount() ? 0 : digit - right.val.GetCount() + 1; i < val.GetCount() && i <= digit; ++i) {
            PRODUCT_TYPE pval = result.val[digit] + val[i] * (PRODUCT_TYPE) right.val[digit - i];
            if (pval >= BASE || pval <= -BASE)
            {
                auto dt = cdiv<long long>(pval, BASE);
                carry += dt.quot;
                pval = dt.rem;
            }
            result.val[digit] = (ELEM_TYPE) pval;
            found = true;
        }
        if (!found)
            break;
    }
    for (; carry > 0; ++digit) {
        auto dt = cdiv<long long>(carry, BASE);
        result.val[digit] = (ELEM_TYPE) dt.rem;
        carry = dt.quot;
    }
    result.correct();
    result.pos = (result.val.GetCount() == 1 && result.val[0] == 0) ? true : (pos == right.pos);
    return result;
}

inline intInf intInf::operator/(const intInf& right) const {
    if (right == 0)
        throw Exc("division by zero");

    intInf Q, R, D = (right.pos ? right : -right), N = (pos ? *this : -*this);
    Q.val.SetCount(N.val.GetCount(), 0);
    for (int i = (int) N.val.GetCount() - 1; i >= 0; --i) {
        R.val.Insert(0, N.val[i]);
        R.correct(true);
        ELEM_TYPE cnt = dInR(R, D);
        R -= D * cnt;
        Q.val[i] += cnt;
    }
    Q.correct();
    Q.pos = (Q.val.GetCount() == 1 && Q.val[0] == 0) ? true : (pos == right.pos);
    return Q;
}

inline intInf intInf::operator%(const intInf& right) const {
    if (right == 0)
        throw Exc("division by zero");

    intInf R, D = (right.pos ? right : -right), N = (pos ? *this : -*this);
    for (int i = (int) N.val.GetCount() - 1; i >= 0; --i) {
        R.val.Insert(0, N.val[i]);
        R.correct(true);
        R -= D * dInR(R, D);
    }
    R.correct();
    R.pos = (R.val.GetCount() == 1 && R.val[0] == 0) ? true : pos;
    return R;
}

inline intInf intInf::operator*(ELEM_TYPE right) const {
    intInf result = *this;
    ELEM_TYPE factor = right < 0 ? -right : right;
    multiplyByDigit(factor, result.val);
    result.correct();
    result.pos = (result.val.GetCount() == 1 && result.val[0] == 0) ? true : (pos == (right >= 0));
    return result;
}

inline bool intInf::operator==(const intInf& right) const {
    if (pos != right.pos || val.GetCount() != right.val.GetCount())
        return false;
    for (int i = val.GetCount() - 1; i >= 0; --i)
        if (val[i] != right.val[i])
            return false;
    return true;
}

inline bool intInf::operator!=(const intInf& right) const {
    if (pos != right.pos || val.GetCount() != right.val.GetCount())
        return true;
    for (int i = (int) val.GetCount() - 1; i >= 0; --i)
        if (val[i] != right.val[i])
            return true;
    return false;
}

inline bool intInf::operator<(const intInf& right) const {
    if (pos && !right.pos)
        return false;
    if (!pos && right.pos)
        return true;
    if (val.GetCount() > right.val.GetCount())
        return pos ? false : true;
    if (val.GetCount() < right.val.GetCount())
        return pos ? true : false;
    for (int i = (int) val.GetCount() - 1; i >= 0; --i) {
        if (val[i] < right.val[i])
            return pos ? true : false;
        if (val[i] > right.val[i])
            return pos ? false : true;
    }
    return false;
}

inline bool intInf::operator<=(const intInf& right) const {
    if (pos && !right.pos)
        return false;
    if (!pos && right.pos)
        return true;
    if (val.GetCount() > right.val.GetCount())
        return pos ? false : true;
    if (val.GetCount() < right.val.GetCount())
        return pos ? true : false;
    for (int i = (int) val.GetCount() - 1; i >= 0; --i) {
        if (val[i] < right.val[i])
            return pos ? true : false;
        if (val[i] > right.val[i])
            return pos ? false : true;
    }
    return true;
}

inline bool intInf::operator>(const intInf& right) const {
    if (pos && !right.pos)
        return true;
    if (!pos && right.pos)
        return false;
    if (val.GetCount() > right.val.GetCount())
        return pos ? true : false;
    if (val.GetCount() < right.val.GetCount())
        return pos ? false : true;
    for (int i = (int) val.GetCount() - 1; i >= 0; --i) {
        if (val[i] < right.val[i])
            return pos ? false : true;
        if (val[i] > right.val[i])
            return pos ? true : false;
    }
    return false;
}

inline bool intInf::operator>=(const intInf& right) const {
    if (pos && !right.pos)
        return true;
    if (!pos && right.pos)
        return false;
    if (val.GetCount() > right.val.GetCount())
        return pos ? true : false;
    if (val.GetCount() < right.val.GetCount())
        return pos ? false : true;
    for (int i = (int) val.GetCount() - 1; i >= 0; --i) {
        if (val[i] < right.val[i])
            return pos ? false : true;
        if (val[i] > right.val[i])
            return pos ? true : false;
    }
    return true;
}

inline void intInf::optimizeSqrtSearchBounds(intInf& lo, intInf& hi) const {
    intInf hdn = 1;
    for (int i = (int) this->numberOfDigits() / 2; i >= 2; --i)
        hdn *= 10;
    if (lo < hdn)
        lo = hdn;
    hdn *= 100;
    if (hi > hdn)
        hi = hdn;
}


inline intInf intInf::intSqrt() const {
    if (*this <= 0)
        throw Exc("intSqrt called for non-positive integer");

    intInf hi = *this / 2 + 1, lo = 0, mid, mid2;
    optimizeSqrtSearchBounds(lo, hi);
    do {
        mid = (hi + lo) / 2; // 8 factor
        mid2 = mid * mid; // 1 factor
        if (mid2 == *this) {
            lo = mid;
            break;
        } else if (mid2 < *this)
            lo = mid;
        else
            hi = mid;
    } while (lo < hi - 1 && mid2 != *this);
    return lo;
}

inline char intInf::digitAt(int i) const {
    ASSERT(numberOfDigits() <= i);

    return (val[i / DIGIT_COUNT] / powersOfTen[i % DIGIT_COUNT]) % 10;
}

inline int intInf::numberOfDigits() const {
    return (val.GetCount() - 1) * DIGIT_COUNT +
        (val.back() > 99999999 ? 9 : (val.back() > 9999999 ? 8 : (val.back() > 999999 ? 7 : (val.back() > 99999 ? 6 :
        (val.back() > 9999 ? 5 : (val.back() > 999 ? 4 : (val.back() > 99 ? 3 : (val.back() > 9 ? 2 : 1))))))));
}

inline String intInf::ToString() const {
	String s;
    if (!pos)
        s << '-';
    bool first = true;
    for (int i = (int) val.GetCount() - 1; i >= 0; --i) {
        if (first) {
            s << val[i];
            first = false;
        } else 
            s << FormatIntDec(val[i], DIGIT_COUNT, '0');
    }
    return s;
}

inline int intInf::size() const {
    return val.GetCount() * sizeof(ELEM_TYPE) + sizeof(bool);
}

inline int intInf::toInt() const {
    if (*this > INT_MAX || *this < INT_MIN)
        throw Exc("out of bounds");

    int result = 0;
    for (int i = (int) val.GetCount() - 1; i >= 0; --i)
        result = result * BASE + val[i];
    return pos ? result : -result;
}

inline long intInf::toLong() const {
    if (*this > LONG_MAX || *this < LONG_MIN)
        throw Exc("out of bounds");

    long result = 0;
    for (int i = (int) val.GetCount() - 1; i >= 0; --i)
        result = result * BASE + val[i];
    return pos ? result : -result;
}

inline long long intInf::toLongLong() const {
    if (*this > LONG_LONG_MAX || *this < LONG_LONG_MIN)
        throw Exc("out of bounds");

    long long result = 0;
    for (int i = (int) val.GetCount() - 1; i >= 0; --i)
        result = result * BASE + val[i];
    return pos ? result : -result;
}

inline unsigned int intInf::toUnsignedInt() const {
    if (!pos || *this > UINT_MAX)
        throw Exc("out of bounds");

    unsigned int result = 0;
    for (int i = (int) val.GetCount() - 1; i >= 0; --i)
        result = result * BASE + val[i];
    return result;
}

inline unsigned long intInf::toUnsignedLong() const {
    if (!pos || *this > ULONG_MAX)
        throw Exc("out of bounds");

    unsigned long result = 0;
    for (int i = (int) val.GetCount() - 1; i >= 0; --i)
        result = result * BASE + val[i];
    return result;
}

inline unsigned long long intInf::toUnsignedLongLong() const {
    if (!pos || *this > ULONG_LONG_MAX)
        throw Exc("out of bounds");

    unsigned long long result = 0;
    for (int i = (int) val.GetCount() - 1; i >= 0; --i)
        result = result * BASE + val[i];
    return result;
}

inline void intInf::truncateToBase() {
    for (int i = 0; i < val.GetCount(); ++i) { // truncate each
        if (val[i] >= BASE || val[i] <= -BASE) {
            auto dt = cdiv<int>(val[i], BASE);
            val[i] = dt.rem;
            if (i + 1 >= val.GetCount())
                val.push_back(dt.quot);
            else
                val[i + 1] += dt.quot;
        }
    }
}

inline bool intInf::equalizeSigns() {
    bool isPositive = true;
    int i = (int) ((val.GetCount())) - 1;
    for (; i >= 0; --i) {
        if (val[i] != 0) {
            isPositive = val[i--] > 0;
            break;
        }
    }

    if (isPositive) {
        for (; i >= 0; --i) {
            if (val[i] < 0) {
                int k = 0, index = i + 1;
                for (; index < val.GetCount() && val[index] == 0; ++k, ++index)
                    ; // count adjacent zeros on left
                //if ((size_t)(index) < val.size() && val[index] > 0)
                { // number on the left is positive
                    val[index] -= 1;
                    val[i] += BASE;
                    for (; k > 0; --k)
                    {
                        val[i + k] = UPPER_BOUND;
                    }
                }
            }
        }
    } else {
        for (; i >= 0; --i) {
            if (val[i] > 0) {
                int k = 0, index = i + 1;
                for (; index < val.GetCount() && val[index] == 0; ++k, ++index)
                    ; // count adjacent zeros on right
                //if ((size_t)(index) < val.size() && val[index] < 0)
                { // number on the left is negative
                    val[index] += 1;
                    val[i] -= BASE;
                    for (; k > 0; --k)
                        val[i + k] = -UPPER_BOUND;
                }
            }
        }
    }
    
    return isPositive;
}

inline void intInf::removeLeadingZeros() {
    for (int i = (int) (val.GetCount()) - 1; i > 0; --i) { // remove leading 0's 
        if (val[i] != 0)
            return;
        else
            val.Remove(i);
    }
}

inline void intInf::correct(bool justCheckLeadingZeros, bool hasValidSign) {
    if (!justCheckLeadingZeros) {
        truncateToBase();
        
        if (equalizeSigns())
            pos = ((val.GetCount() == 1 && val[0] == 0) || !hasValidSign) ? true : pos;
        else {
            pos = hasValidSign ? !pos : false;
            for (int i = 0; i < val.GetCount(); ++i)
                val[i] = ::abs(val[i]);
        }
    }
    removeLeadingZeros();
}

inline void intInf::fromString(const String& s) {
    pos = true;
    val.Clear();
    val.Reserve(s.GetCount() / DIGIT_COUNT + 1);
    int i = s.GetCount() - DIGIT_COUNT;
    for (; i >= 0; i -= DIGIT_COUNT)
        val << atoi(s.Mid(i, DIGIT_COUNT));
    if (i > -DIGIT_COUNT) {
        String ss = s.Left(i + DIGIT_COUNT);
        if (ss.GetCount() == 1 && ss[0] == '-')
            pos = false;
        else
            val.push_back(atoi(ss));
    }
    if (val.back() < 0) {
        val.back() = -val.back();
        pos = false;
    }
    correct(true);
}

inline ELEM_TYPE intInf::dInR(const intInf& R, const intInf& D) {
    ELEM_TYPE min = 0, max = UPPER_BOUND;
    while (max > min) {
        ELEM_TYPE avg = max + min;
        auto dt = cdiv<int>(avg, 2);
        avg = dt.rem ? (dt.quot + 1) : dt.quot;
        intInf prod = D * avg;
        if (R == prod)
            return avg;
        else if (R > prod)
            min = avg;
        else
            max = avg - 1;
    }
    return min;
}

inline void intInf::multiplyByDigit(ELEM_TYPE factor, Vector<ELEM_TYPE>& val) {
    ELEM_TYPE carry = 0;
    for (int i = 0; i < val.GetCount(); ++i) {
        PRODUCT_TYPE pval = val[i] * (PRODUCT_TYPE) factor + carry;
        if (pval >= BASE || pval <= -BASE) {
            auto dt = cdiv<long long>(pval, BASE);
            carry = (ELEM_TYPE) dt.quot;
            pval = dt.rem;
        } else 
            carry = 0;
        val[i] = (ELEM_TYPE) pval;
    }
    if (carry > 0)
        val.push_back(carry);
}

}

#endif
