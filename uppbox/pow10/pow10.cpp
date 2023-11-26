#include <Core/Core.h>

using namespace Upp;

#ifdef __SIZEOF_INT128__     // GNU C
 static inline
 uint64_t mulhi64(uint64_t a, uint64_t b) {
     unsigned __int128 prod =  a * (unsigned __int128)b;
     return prod >> 64;
 }
 
static inline
uint64 mul64x64(uint64 a, uint64 b, uint64& hi)
{
	unsigned __int128 prod =  (unsigned __int128)a * b;
	hi = prod >> 64;
	return prod;
}

#elif defined(_M_X64) || defined(_M_ARM64)     // MSVC
   // MSVC for x86-64 or AArch64
   // possibly also  || defined(_M_IA64) || defined(_WIN64)
   // but the docs only guarantee x86-64!  Don't use *just* _WIN64; it doesn't include AArch64 Android / Linux

  // https://docs.microsoft.com/en-gb/cpp/intrinsics/umulh
  #include <intrin.h>
  #define mulhi64 __umulh

#elif defined(_M_IA64) // || defined(_M_ARM)       // MSVC again
  // https://docs.microsoft.com/en-gb/cpp/intrinsics/umul128
  // incorrectly say that _umul128 is available for ARM
  // which would be weird because there's no single insn on AArch32
  #include <intrin.h>
  static inline
  uint64_t mulhi64(uint64_t a, uint64_t b) {
     unsigned __int64 HighProduct;
     (void)_umul128(a, b, &HighProduct);
     return HighProduct;
  }

#else

# undef HAVE_FAST_mul64
  uint64_t mulhi64(uint64_t a, uint64_t b);  // non-inline prototype
  // or you might want to define @craigster0's version here so it can inline.
#endif

// unsigned char _addcarry_u64 (unsigned char c_in, unsigned __int64 a, unsigned __int64 b, unsigned __int64 * out)

struct Int64_ {
	uint64 value;

	static int bits()                    { return 64; }

	int     significant_bits() const     { return SignificantBits64(value); }

	uint64& hi64()       { return value; }
	uint64& lo64()       { return value; }

	byte addc(const Int64_& a, byte carry) {
		return _addcarry_u64(carry, value, a.value, &value);
	}
	byte add64(const uint64& a) {
		return _addcarry_u64(0, value, a, &value);
	}
	Int64_ operator+(Int64_ a) const {
		Int64_ r;
		r.value = value + a.value;
		return r;
	}
	Int64_ operator*(Int64_ a) const {
		Int64_ r;
		r.value = value * a.value;
		return r;
	}
	Int64_ mul(Int64_ a, Int64_& hi) const
	{
		Int64_ ret;
		ret.value = mul64x64(value, a.value, hi.value);
		return ret;
	}
	
	Int64_ mulhi(Int64_ a) const {
		Int64_ r;
		r.value = mulhi64(value, a.value);
		return r;
	}
	uint64 shl(int shift, uint64 carry) {
		ASSERT(shift < 64);
	//	DDUMP(shift);
	//	DDUMPHEX(carry);
	//	DDUMPHEX(value);
		if(shift == 0) return 0; // value >> 64 undefined
		uint64 rem = value >> (64 - shift);
		value = (value << shift) | carry;
	//	DDUMPHEX(value);
	//	DDUMPHEX(rem);
		return rem;
	};
	uint64 shr(int shift, uint64 carry) {
		ASSERT(shift < 64);
		if(shift == 0) return 0; // value << 64 undefined
		uint64 rem = value << (64 - shift);
		value = (value >> shift) | carry;
		return rem;
	};
	void invert() {
		value = value ^ 0xffffffffffffffff;
	}
	int  Compare(Int64_ b) const {
		return SgnCompare(value, b.value);
	}
	String ToHex() const { return Sprintf("%016llx", value); }
	
	Int64_(uint64 x = 0) { value = x; }
};

template <class T>
struct IntLH : Comparable<IntLH<T>> {
	T l, h;

	IntLH() {}
	IntLH(T l) : l(l) {}
	IntLH(T l, T h) : l(l), h(h) {}
	IntLH(uint64 x) : l(x) {}

	typedef IntLH<IntLH<T>> Int2LH;
	static int bits()                    { return 2 * T::bits(); }
	int     significant_bits() const     { int q = h.significant_bits(); return q ? q + T::bits() : l.significant_bits(); }
	uint64& hi64()                       { return h.hi64(); }
	uint64& lo64()                       { return l.lo64(); }
	bool    hibit()                      { return h.hi64() & 0x8000000000000000; }
	IntLH   lo() const                   { return l; }
	IntLH   hi() const                   { return h; }

	int    addc(const IntLH& b, int carry) { return h.addc(b.h, l.addc(b.l, carry)); }
	int    add64(uint64 b)                 { return h.add64(l.add64(b)); }
	uint64 shl(int shift = 1, uint64 carry = 0) {
		if(shift >= T::bits()) {
			h = l;
			l = carry;
			return h.shl(shift - T::bits(), 0);
		}
		return h.shl(shift, l.shl(shift, carry));
	};
	uint64 shr(int shift = 1, uint64 carry = 0) {
		if(shift >= T::bits()) {
			l = h;
			h = carry;
			return l.shr(shift - T::bits(), 0);
		}
		return l.shr(shift, h.shr(shift, carry));
	};
	void   invert() {
		l.invert();
		h.invert();
	}
	IntLH  mulhi(IntLH b) const {
		IntLH a_lo = lo();
		IntLH a_hi = hi();
		IntLH b_lo = b.lo();
		IntLH b_hi = b.hi();
		
		IntLH a_x_b_mid = a_hi * b_lo;
		IntLH b_x_a_mid = b_hi * a_lo;

		return (a_hi * b_hi) + a_x_b_mid.hi() + b_x_a_mid.hi() +
		       (a_x_b_mid.lo() + b_x_a_mid.lo() + (a_lo * b_lo).hi()).hi();
	}

	IntLH  mul(IntLH b, IntLH& hi) const {
		IntLH mid1, mid2, lo;
		mid1.l = h.mul(b.l, mid1.h);
		mid2.l = b.h.mul(l, mid2.h);
		lo.l = l.mul(b.l, lo.h);
		IntLH rh = IntLH(mid1.l) + mid2.l + lo.h;

		hi.l = h.mul(b.h, hi.h);
		hi = hi + mid1.h + mid2.h + rh.h;
		IntLH ret;
		ret.l = lo.l;
		ret.h = rh.l;
		return ret;
	}

	IntLH operator*(IntLH b) const {
		IntLH mid1, mid2, lo;
		mid1.l = h.mul(b.l, mid1.h);
		mid2.l = b.h.mul(l, mid2.h);
		lo.l = l.mul(b.l, lo.h);
		IntLH rh = IntLH(mid1.l) + mid2.l + lo.h;
		IntLH ret;
		ret.l = lo.l;
		ret.h = rh.l;
		return ret;
	}

	IntLH operator+(IntLH b) const {
		IntLH r = *this;
		r.addc(b, 0);
		return r;
	}

	IntLH operator-(IntLH b) const {
		IntLH r = *this;
		b.invert();
		b.add64(1);
		r = r + b;
		return r;
	}

	IntLH operator<<(int shift) const {
		IntLH r = *this;
		r.shl(shift, 0);
		return r;
	}

	IntLH operator>>(int shift) const {
		IntLH r = *this;
		r.shr(shift, 0);
		return r;
	}

/*
void bindivmod128(uint128 M, uint128 N, uint128& Q, uint128 &R)
{
    Q.Hi = Q.Lo = 0;
    size_t Shift = nlz128(N) - nlz128(M);
    shiftleft128(N, Shift, N);

    do
    {
        shiftleft128(Q, 1, Q);
        if(compare128(M, N) >= 0)
        {
            sub128(M, N, M);
            Q.Lo |= 1;
        }

        shiftright128(N, 1, N);
    }while(Shift-- != 0);

    R.Hi = M.Hi;
    R.Lo = M.Lo;
}
*/
	IntLH div(IntLH n, IntLH& rem) const {
		IntLH q;
		rem = *this;
		if(n > *this)
			return 0;
		int count = significant_bits() - n.significant_bits();
	//	DDUMP(count);
	//	DDUMP(n.ToHex());
	//	DDUMP(rem.ToHex());
		n = n << count;
	//	DDUMP(n.ToHex());
		do {
			q = q << 1;
			if(rem >= n) {
				rem = rem - n;
				q.lo64() |= 1;
			}
			n = n >> 1;
		}
		while(count-- != 0);
	/*
		for(;;) {
			IntLH n1 = n << 1;
			if(n1 < n)
				break;
			if(n1 > rem)
				break;
			n = n1;
			count++;
		}
		rem = *this;
		while(count-- >= 0) {
			q = q << 1;
			if(rem >= n) {
				rem = rem - n;
				q.ls64() |= 1;
			}
			n = n >> 1;
		}
	*/
		return q;
	}
	
	IntLH operator/(IntLH b) const
	{
		IntLH rem;
		return div(b, rem);
	}
	
	String ToString() const {
		Vector<char> h;
		IntLH a = *this;
		do {
			IntLH rem;
			a = a.div(10, rem);
			h.Add(rem.lo64() + '0');
		}
		while(a > 0);
		Reverse(h);
		return String(h.begin(), h.GetCount());
	}

	String ToHex() const {
		return h.ToHex() + "," + l.ToHex();
	}

	int Compare(const IntLH& b) const {
		int q = SgnCompare(h, b.h);
		if(q) return q;
		return SgnCompare(l, b.l);
	}
};

typedef IntLH<Int64_>   uint128;
typedef IntLH<uint128>  uint256;
typedef IntLH<uint256>  uint512;
typedef IntLH<uint512>  uint1024;
typedef IntLH<uint1024> uint2048;
typedef IntLH<uint2048> uint4096;
typedef IntLH<uint4096> uint8192;
typedef IntLH<uint8192> uint16384;
typedef IntLH<uint16384> uint32768;

template <class T>
struct FPN {
	T    mantissa;
	int  exponent;
	bool sign;
	
	void Normalize()
	{
		int shift = mantissa.bits() - mantissa.significant_bits();
		if(shift) {
			mantissa.shl(shift);
			exponent -= shift;
		}
	}
	
	FPN& operator*=(const FPN& b) {
		typename T::Int2LH x, y;
		x.l = mantissa;
		y.l = b.mantissa;
		x = x * y;
		exponent += b.exponent;
		if(!x.h.hibit()) {
			x = x << 1;
			exponent--;
		}
		mantissa = x.h;
		sign = (int)sign ^ (int)b.sign;
		return *this;
	}
	FPN& operator/=(const FPN& b) {
		typename T::Int2LH x, y;
		x.h = mantissa;
		y.l = b.mantissa;
	//	DDUMP(x.ToHex());
	//	DDUMP(y.ToHex());
		x = x / y;
	//	DDUMP(x.ToHex());

		int shift = x.bits() - x.significant_bits();
	//	DDUMP(shift);
		x.shl(shift);
	//	DDUMP(x.ToHex());
		mantissa = x.h;
		
	//	DDUMP(exponent);
	///	DDUMP(b.exponent);
	//	DDUMP(shift);
	//	DDUMP(mantissa.bits());
		
		exponent = exponent - shift + mantissa.bits() - b.exponent;
	//	DDUMP(exponent);
		
		sign = (int)sign ^ (int)b.sign;
		return *this;
	}
	FPN() { exponent = 0; mantissa = 0; }
	FPN(double x) {
		union { // get dirty with IEEE double!
			uint64 u;
			double d;
		} h;
		h.d = x;
		mantissa.hi64() = (h.u << 11) | ((uint64)1 << 63);
		sign = h.u & ((uint64)1 << 63);
		exponent = ((h.u >> 52) & 2047) - 1022;
	}
	double ToDouble() {
		union { // get dirty with IEEE double!
			uint64 u;
			double d;
		} h;
		int ex = exponent;
		uint64 m = mantissa.hi64();

		h.u = m >> 11;
		if(m & 1024) { // round up?
			h.u++;
			if(h.u & ((uint64)1 << 53)) { // overflow while rounding, renormalize; test this!
				h.u >>= 1;
				ex++;
			}
		}
		
		h.u = h.u & (((uint64)1 << 52) - 1) | (uint64(ex + 1022) << 52);
		
		return h.d;
	}

};


uint256 x, y;

typedef FPN<uint1024> f1024;
typedef FPN<uint2048> f2048;
typedef FPN<uint4096> f4096;

f1024 pow10t[701];

void MakeTheTable()
{
	f1024 one = 1;
	f1024 ten = 10;
	pow10t[350 + 0] = one;
	pow10t[350 + 1] = ten;
	f1024 h = one;
	h /= ten;
	pow10t[350 - 1] = h;
	for(int i = 2; i <= 350; i++) {
		int q = i / 2;
		f1024 x = pow10t[350 + q];
		x *= pow10t[350 + (i - q)];
		pow10t[350 + i] = x;
		f1024 ix;
		ix = one;
		ix /= x;
		pow10t[350 - i] = ix;
	}
	
	for(int i = 0; i < 701; i++)
		LOG("{ " << pow10t[i].exponent << ", "
		    << "0x" << Format64Hex(pow10t[i].mantissa.hi64())
		    << ", "
		    << "0x" << Format64Hex((pow10t[i].mantissa << 64).hi64())
		    << " }, // " << i - 350 << " " << pow10t[i].ToDouble());
}

CONSOLE_APP_MAIN
{
	MakeTheTable();
	return;
}
