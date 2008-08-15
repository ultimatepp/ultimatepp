#ifndef _numeric_decimal_h_
#define _numeric_decimal_h_

#include <Core/Core.h>

using namespace UPP;

#define DECIMAL_SIZE 3*9
#define DECIMAL_PRECISION 1*9

#define _D_SIZE_ (DECIMAL_SIZE/9+((DECIMAL_SIZE%9>0)?1:0))
#define _D_PREC_ (DECIMAL_PRECISION/9+((DECIMAL_PRECISION%9>0)?1:0))

class decimal;

#define DEFAULT_ROUNDING	decimal::ROUND_UP

decimal abs(decimal d);

const int DECIMAL_V	=	11000;

class decimal : AssignValueTypeNo<decimal, DECIMAL_V, Comparable<decimal, Moveable<decimal> > > {
protected:
	int32 number[_D_SIZE_];
	short sign;
	int print_precision, rounding_mode;
	bool parse_string, trim_trailing_zeroes;

	decimal& ShiftLeft();
	decimal& ShiftRight();
	
public:

	enum ROUND_MODES { ROUND_DOWN, ROUND_UP, ROUND_HALF_EVEN };

	static decimal ZERO;
	static decimal ONE;
	static decimal TWO;
	static decimal TEN;

	decimal();

	void PrintPrecision(int precision);
	int PrintPrecision();
	
	void RoundingMode(int rounding);
	int RoundingMode();
	
	void TrimZeros(bool b);
	bool TrimZeros();

	void FromString(String s);		
	String ToString() const;
		
	decimal& Round(int precision=Null, int rounding=Null);
		
	decimal& operator=(decimal d);
	
	 decimal& operator=(const Nuller n) {
		number[0]=1111111111;
		return *this;
	}	
	
	template <class T>
	 decimal& operator=(T n) {
		decimal temp(AsString(n));
		return (*this=temp);
	}
	
	int CompareTo(decimal d) const;
	
	 bool operator==(decimal &d);
	
	template<class T>
	 bool operator==(T d) {
		return (CompareTo(decimal(d))==0);
	}	

	 bool operator!=(decimal &d);
	
	template<class T>
	 bool operator!=(T d) {
		return (CompareTo(decimal(d))!=0);
	}	
	
	 bool operator>=(decimal &d);
	
	template<class T>
	bool operator>=(T d) {
		return (CompareTo(decimal(d))>=0);
	}	
	
	bool operator>(decimal &d);
	
	template<class T>
	 bool operator>(T d) {
		return (CompareTo(decimal(d))>0);
	}	
	
	bool operator<=(decimal &d);
	
	template<class T>
	bool operator<=(T d) {
		return (CompareTo(decimal(d))<=0);
	}	
	
	bool operator<(decimal &d);
	
	template<class T>
	bool operator<(T d) {
		return (CompareTo(decimal(d))<0);
	}	

	decimal& operator+=(decimal &d);
	
	template <class T>
	decimal& operator+=(T n) {
		decimal temp(n);
		return (*this+=temp);
	}
	
	decimal operator+(decimal &d);
	
	template <class T>
	decimal operator+(T n) {
		decimal temp(*this);
		return (temp+=decimal(n));
	}		
	
	decimal operator++(int);
	
	decimal& operator++();
	
	decimal& operator-=(decimal &d);
		
	template <class T>
	decimal& operator-=(T n) {
		decimal temp(n);
		return (*this-=temp);
	}	
	
	decimal operator-(decimal &d);
	
	template <class T>
	decimal operator-(T n) {
		decimal temp(*this);
		return (temp-=decimal(n));
	}	
	
	decimal operator-();

	decimal operator--(int);
	
	decimal& operator--();
		
	decimal& operator*=(decimal &d);
	
	template <class T>
	decimal& operator*=(T n) {
		decimal temp(n);
		return (*this*=temp);
	}		
	
	decimal operator*(decimal &d);
	
	template <class T>
	decimal operator*(T n) {
		decimal temp(*this);
		return (temp*=decimal(n));
	}
	
	decimal& operator/=(decimal &d);
	
	decimal& Sqrt();

	template <class T>
	decimal& operator/=(T n) {
		decimal temp(n);
		return (*this/=temp);
	}		
	
	decimal operator/(decimal &d);
	
	template <class T>
	decimal operator/(T n) {
		decimal temp(*this);
		return (temp/=decimal(n));
	}

	decimal& operator<<=(int n);
	
	decimal operator<<(int n);

	decimal& operator>>=(int n);
	
	decimal operator>>(int n);

	decimal& operator^=(int n);
	
	decimal operator^(int n);
	
	bool isnull() const;
		
	decimal& Absolute();
	
	decimal& Floor();
	
	decimal& Ceil();
	
	bool IsNullInstance() const;
	
	operator Value() const;

	decimal(const Value& v, int precision=DECIMAL_PRECISION, int rounding=DEFAULT_ROUNDING);
		
	int Compare(const Value& b) const;

	void Serialize(Stream &s);
	
	unsigned GetHashValue() const;

	operator int() { 
		decimal temp=floor(abs(*this));
		int i=StrInt(temp.ToString());
		i*=sign;
		return i;
	}	
	
	operator int64() {
		decimal temp=floor(abs(*this));
		int64 i=stou64(temp.ToString());
		i*=sign;
		return i;
	}		

	operator double() {
		return StrDbl(ToString());
	}
	
	operator String() {
		return ToString();
	}		
};

static decimal operator+(Value t, decimal d);

static decimal operator-(Value t, decimal d);

static decimal operator*(Value t, decimal d);

static decimal operator/(Value t, decimal d);

decimal floor(decimal d);
decimal ceil(decimal d);
decimal sqrt(decimal d);
decimal round(decimal d, int precision, int rounding=DEFAULT_ROUNDING);


INITBLOCK
{
    RichValue<decimal>::Register();
}

#endif
