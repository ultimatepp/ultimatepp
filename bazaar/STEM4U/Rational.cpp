#include "STEM4U.h"

namespace Upp {

Rational abs(const Rational &rat) {
	if (rat.num >= 0)
		return rat;
	else
		return -rat;
}

Rational pow(const Rational &rat, int e) {
	ASSERT(e >= 0);
	Rational ret = 1;
	for (; e > 0; --e)	
		ret *= rat;
	return ret;
}

Rational sqrt(const Rational &rat) {
	Rational ret = Rational(rat.num.intSqrt(), rat.den.intSqrt());
	return ret;
}

String FormatRational(const Rational &d, int numDec) {
	if (IsNull(numDec)) {
		if (d.den == 1)
			return d.num.ToString();
		else if (d.num == 0)
			return "0";
		else {
			Rational dd = d;
			dd.Simplify();
			String strden = dd.den.ToString();
			if (strden == "1")
				return dd.num.ToString();
			else
				return dd.num.ToString() + "/" + strden;
		}
	} else {
		String strDec('0', numDec);
		strDec = "1" + strDec;
		intInf ret = (d.num*intInf(strDec.Begin()))/d.den;
		bool neg = ret < 0;
		ret = abs(ret);
		int ndig = int(ret.numberOfDigits());
		String rets = ret.ToString();
		if (ndig <= numDec) 
			rets = "0." + String('0', numDec - ndig) + rets;
		else
			rets.Insert(ndig - numDec, '.');
		if (neg)
			rets = "-" + rets;
		return rets;
	}
}

Rational operator+(const Rational& left, const Rational& right) {
	Rational ret;
	if (left.den == right.den) {
		ret.num = left.num + right.num;
		ret.den = left.den;
	} else {
		ret.num = left.num*right.den + left.den*right.num;
		ret.den = left.den*right.den;
	}
	return ret;
}

Rational operator-(const Rational& left, const Rational& right) {
	Rational ret;
	if (left.den == right.den) {
		ret.num = left.num - right.num;
		ret.den = left.den;
	} else {
		ret.num = left.num*right.den - left.den*right.num;
		ret.den = left.den*right.den;
	}
	return ret;
}

Rational operator*(const Rational& left, const Rational& right) {
	Rational ret;
	ret.num = left.num*right.num; 	
	ret.den = left.den*right.den; 	
	return ret;
}

Rational operator/(const Rational& left, const Rational& right) {
	Rational ret;
	ret.num = left.num*right.den; 	
	ret.den = left.den*right.num; 	
	return ret;
}

Rational Rational::Simplify(bool full) {
	if (den < 0) {
		num = -num;
		den = -den;
	}
	if (num == 0) {
		den = 1;
		return *this;
	}
	if (den == 0)
		throw std::domain_error("division by zero");

	const int prime[] = {2*2*2*2*2*2*2*2*2*2*2*2*2*2*2*2, 2*2*2*2*2*2*2*2, 2*2*2*2, 2, 
						 3*3*3*3*3*3*3*3*3*3*3*3*3*3*3, 3*3*3*3*3*3*3*3*3, 3*3*3, 3,
						 5*5*5*5*5*5*5*5*5*5, 5*5*5*5*5, 5, 
						 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 
						 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 
						 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 
						 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 
						 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 
						 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 
						 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 
						 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 
						 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 
						 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 
						 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 
						 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 
						 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 
						 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 
						 947, 953, 967, 971, 977, 983, 991, 997, 0};
	for (int i = 0; prime[i] > 0; ++i) 
		SimplifyVal(prime[i]);
	if (full) {
		for (intInf div = 1000; true; ++div) {
			if (num < div || den < div)
				break;
			SimplifyVal(div);
		}
	}
	if (num < den) {
		if (den%num == 0) {
			den /= num;
			num = 1;
		}
	}
	return *this;
}

}