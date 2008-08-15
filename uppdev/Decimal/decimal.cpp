#include "decimal.h"

decimal::decimal() {
	ASSERT(_D_SIZE_>_D_PREC_);
	parse_string=true;
	memsetd(number, 0, _D_SIZE_);
	sign=1;
	trim_trailing_zeroes=true;
	print_precision=DECIMAL_PRECISION;
	rounding_mode=DEFAULT_ROUNDING;
	parse_string=true;
}

void decimal::FromString(String s) {
	int i=0, idx=-1, j=_D_SIZE_-_D_PREC_, length;

	memsetd(number, 0, _D_SIZE_);
	sign=1;
	s=TrimLeft(TrimRight(s));
	
	if(parse_string) {		
		length=s.GetCount();			
		if(s[i]=='-' || s[i]=='+') { length--; i++; }
		while(i<s.GetCount() && IsDigit(s[i])) i++;
		if(s[i]=='.') idx=i++;
		while(i<s.GetCount() && IsDigit(s[i])) i++;
					
		if(idx>=0) length--;
		if(i<s.GetCount() || (length>_D_SIZE_*9)) { throw Exc("NUMBER TOO BIG TO FIT IN SPECIFIED FORMAT"); return; } // OVERFLOW
		if(idx>=0 && (s.GetCount()-idx-1)>_D_PREC_*9) { throw Exc("NUMBER TOO BIG TO FIT IN SPECIFIED FORMAT"); return; } // OVERFLOW
	}
	if(s[0]=='-' || s[0]=='+') {
		if(s[0]=='-') sign=-1;
		s.Remove(0);
	}
	Vector<String> parts=Split(s, '.');
	
	if(parts.GetCount()>0) {
		String temp=parts[0];
		length=temp.GetCount();
		while(length>=9) { number[--j]=StrInt(temp.Right(9)); temp.Trim(length-=9); }
		if(temp.GetCount()>0) number[--j]=StrInt(temp);			
	}
	if(parts.GetCount()>1) {
		String temp=parts[1];
		j=_D_SIZE_-_D_PREC_;
		length=temp.GetCount();
		while(length>=9) { number[j++]=StrInt(temp.Left(9)); length-=9; temp=temp.Mid(9); }
		if(temp.GetCount()>0) number[j++]=StrInt(temp+String('0', 9-temp.GetCount()));			
	}		
	j=0;
	for(i=0;i<_D_SIZE_;i++) if(number[i]!=0) j++;
	if(j==0) sign=1;
}

void decimal::PrintPrecision(int precision) {
	print_precision=precision;
}

int decimal::PrintPrecision() {
	return print_precision;
}

void decimal::RoundingMode(int rounding) {
	rounding_mode=rounding;
}

int decimal::RoundingMode() {
	return rounding_mode;
}

void decimal::TrimZeros(bool b) {
	trim_trailing_zeroes=b;
}

bool decimal::TrimZeros() {
	return trim_trailing_zeroes;
}

#define ROUND_DECIMAL(PREC, ROUND_MODE) \
		switch(ROUND_MODE) { \
			case ROUND_DOWN: { \
					idx=s.GetCount()-_D_PREC_*9+PREC-1; \
					if(s[idx+1]>'5') s.Set(idx, s[idx]+1); \
					while(idx>0 && s[idx]>'9') { s.Set(idx, '0'); s.Set(--idx, s[idx]+1); } \
				} \
				break; \
			case ROUND_UP: { \
					idx=s.GetCount()-_D_PREC_*9+PREC-1; \
					if(s[idx+1]>='5') s.Set(idx, s[idx]+1); \
					while(idx>0 && s[idx]>'9') { s.Set(idx, '0'); s.Set(--idx, s[idx]+1); } \
				} \
				break; \
			case ROUND_HALF_EVEN: { \
					int temp=0; \
					idx=s.GetCount()-_D_PREC_*9+PREC-1; \
					if(s[idx+1]>'5') s.Set(idx, s[idx]+1); \
					else \
					if(s[idx+1]=='5') { \
						if((s[idx]-'0')%2==1) s.Set(idx, s[idx]+1); \
						else { \
							for(int j=idx+2;j<s.GetCount();j++) temp+=(s[j]-'0'); \
							if(temp>0) s.Set(idx, s[idx]+1); \
						} \
					} \
					while(idx>0 && s[idx]>'9') { s.Set(idx, '0'); s.Set(--idx, s[idx]+1); } \
				} \
				break;		} 			

decimal& decimal::Round(int precision, int rounding) {
	
	if(IsNull(precision)) precision=print_precision;
	if(IsNull(rounding))  rounding=rounding_mode;
	
	if((precision<_D_PREC_*9)) {
		String s="";
		int i=0, idx;
		if(sign<0) s="-";
		for(;i<_D_SIZE_;i++) s+=Format("%09i", (int64)number[i]);
		
		ROUND_DECIMAL(precision, rounding);

		for(i=idx+1;i<s.GetCount();i++) s.Set(i, '0');
		for(i=0;i<_D_SIZE_;i++) { 
			number[i]=StrInt(s.Left(9)); 
			s=s.Mid(9); 
		}
	}
	return *this;
}
	
String decimal::ToString() const {
	if(isnull()) return "";
	String s="";
	int i=0, idx;
	if(sign<0) s="-";
	for(;i<_D_SIZE_;i++) s+=Format("%09i", (int64)number[i]);
	if(_D_PREC_>0) {
		if(s!="0") {
			if((print_precision<_D_PREC_*9) || IsNull(print_precision))
			ROUND_DECIMAL(print_precision, rounding_mode);
			
			s.Insert(s.GetCount()-_D_PREC_*9, '.');
			s=s.Left(s.GetCount()-_D_PREC_*9+print_precision);
			if(trim_trailing_zeroes) {
				int i=s.GetCount();
				while(s[i-1]=='0') i--;
				if(s[i-1]=='.') i--;
				s.Trim(i);
			}			
		}
		else if(!trim_trailing_zeroes) s+="."+String('0', _D_PREC_*9);
	}	
	int j=0;	
	if(s[0]=='-') j=1;
	i=j;
	while(i<s.GetCount() && s[i]=='0') i++;
	if(s[i]=='.' && i>0) i--;
	s.Remove(j, i-j);
	if(sign<0) {
		i=1;
		while(i<s.GetCount() && (s[i]=='0' || s[i]=='.')) i++;
		if(i>=s.GetCount()) s=s.Mid(1);
	}
	if(*s.Last()=='.') s=s.Left(s.GetCount()-1);	
	
//	Round();
//	parse_string=false; 
/*	FromString(s); 
	parse_string=true;*/
	
	return s;
}
	
decimal& decimal::operator=(decimal d) {
	memcpy(number, d.number, _D_SIZE_*sizeof(int));
	sign=d.sign;
	
	return *this;
}

int decimal::CompareTo(decimal d) const {
	if(sign!=d.sign) 
		if(sign>d.sign) return 1; 
		if(sign<d.sign) return -1;

	int i=0;
	while(i<_D_SIZE_ && number[i]==d.number[i]) i++;
	if(i>=_D_SIZE_) return 0;
	if(number[i]>d.number[i]) return 1;
	return -1;
}

bool decimal::operator==(decimal &d) {
	return (CompareTo(d)==0);
}

bool decimal::operator!=(decimal &d) {
	return (CompareTo(d)!=0);
}	

bool decimal::operator>=(decimal &d) {
	return (CompareTo(d)>=0);
}

bool decimal::operator>(decimal &d) {
	return (CompareTo(d)>0);
}

bool decimal::operator<=(decimal &d) {
	return (CompareTo(d)<=0);
}

bool decimal::operator<(decimal &d) {
	return (CompareTo(d)<0);
}	

decimal& decimal::operator+=(decimal &d) {
	if(sign==d.sign) {
		decimal temp(d);
		int carry=0;
		
		for(int i=_D_SIZE_-1;i>=0;i--) {
			number[i]+=(temp.number[i]+carry);
			if(number[i]>999999999) { number[i]-=(int32)1000000000; carry=1; }
			else carry=0;
		}
		if(carry>0) { throw Exc("NUMERIC OVERFLOW"); return *this; } // OVERFLOW
		
		return *this;
	}
	if(sign<0) { sign=1; decimal temp(*this); *this=d; return (*this-=temp);  }
	decimal temp(d); temp.sign=1; 
	return (*this-=temp);
}

decimal decimal::operator+(decimal &d) {
	decimal temp(*this);
	return (temp+=d);
}

decimal decimal::operator++(int) {
	decimal temp(*this);
	*this+=1;
	return temp;
}

decimal& decimal::operator++() {
	return (*this+=1);
}	

decimal& decimal::operator-=(decimal &d) {
	if(sign==d.sign && sign==1) {
		decimal temp(d);
		if(CompareTo(d)<0) { decimal temp2(*this); *this=temp; temp=temp2; sign*=-1; }

		int carry=0;
		
		for(int i=_D_SIZE_-1;i>=0;i--) {
			number[i]-=(temp.number[i]+carry);
			if(number[i]<0) { number[i]+=(int32)1000000000; carry=1; }
			else carry=0;
		}
		
		return *this;
	}
	if(sign==d.sign) { decimal temp(*this); temp.sign=1; *this=d; sign=1; return (*this-=temp); }
	if(sign<0) { decimal temp(d); temp.sign=1; sign=1; *this+=temp; sign=-1; return *this; }
	decimal temp(d); temp.sign=1; 
	return (*this+=temp);
}	
	
decimal decimal::operator-(decimal &d) {
	decimal temp(*this);
	return (temp-=d);
}

decimal decimal::operator-() {
	decimal temp(*this);
	temp.sign*=-1;
	return temp;
}

decimal decimal::operator--(int) {
	decimal temp(*this);
	*this-=1;
	return temp;
}

decimal& decimal::operator--() {
	return (*this-=1);
}	
	
decimal& decimal::operator*=(decimal &d) {
	int64 accum[(_D_SIZE_ << 1)-1], carry=0;
	int64 temp, mult;
	memsetd(accum, 0, ((_D_SIZE_ << 1)-1) << 1);
	
	int idx=(_D_SIZE_ << 1)-2;
	for(int i=_D_SIZE_-1;i>=0;i--) {
		mult=d.number[i];
		for(int j=_D_SIZE_-1,k=idx;j>=0;j--,k--) {
			temp=number[j];
			accum[k]=temp*mult+accum[k];
		}
		idx--;
	}
	for(int i=(_D_SIZE_ << 1)-2;i>=0;i--) {
		int64 temp=accum[i]+carry;
		if(temp>=1000000000) { carry=temp/1000000000; accum[i]=temp%1000000000; }
		else { carry=0; accum[i]=temp; }
	}		
	for(int i=0;i<_D_SIZE_;i++) number[i]=(int32)accum[(_D_SIZE_-1-_D_PREC_)+i];
	sign*=d.sign;
	if(carry>0) { throw Exc("NUMERIC OVERFLOW"); return *this; } // OVERFLOW
	
	return *this;
}

decimal decimal::operator*(decimal &d) {
	decimal temp(*this);
	return (temp*=d);
}

decimal& decimal::operator/=(decimal &d) {
	if(d.CompareTo(decimal(0))==0) { throw Exc("DIVISION BY ZERO!"); return *this; }
	if(d.CompareTo(decimal(1))==0) return *this;
	if(d.CompareTo(decimal(-1))==0) { sign*=-1; return *this; }
	if(d.CompareTo(decimal(*this))==0) { return decimal::ONE; }
	if(d.CompareTo(decimal(2))==0) { ShiftRight(); return *this; }
	if(d.CompareTo(decimal(4))==0) { ShiftRight(); return *this; }
	
	int _sign=sign;
	sign=1;
	decimal l, m(*this), v;
	bool found=false, repeat=false;
	int cmp=0;
	
	if(abs(d)<ONE) {
		int i=0;
		String temp=d.ToString();
		temp=temp.Mid(temp.Find('.')+1);
		m=10;
		while(temp[i++]=='0') m*=10;
	}
	
	m>>=1; l=m;
	while(!found) {
		v=m;
		v*=d;
		cmp=CompareTo(v);
		repeat=(l.CompareTo(decimal::ZERO)==0);
		found=(cmp==0) || repeat;
		
		if(!found) {
			l>>=1;
			if(cmp<0) m-=l;
			else m+=l;
		}
	}
	
	_sign=_sign*d.sign;
	*this=m;
	sign=_sign;

	return *this;
}

decimal& decimal::Sqrt() {
	if(sign<0) { throw Exc("NO SQRT FOR NEGATIV NUMBERS!"); return *this; }
	if(CompareTo(decimal(0))==0) { *this=ZERO; return *this; }
	if(CompareTo(decimal(1))==0) return *this;

	decimal l, m(*this), v;
	bool found=false, repeat=false;
	int cmp=0;

	if(m<ONE) m=1;
	m>>=1; l=m;
	while(!found) {
		v=m;
		v*=v;
		cmp=CompareTo(v);
		repeat=(l.CompareTo(decimal::ZERO)==0);
		found=(cmp==0) || repeat;
		
		if(!found) {
			l>>=1;
			if(cmp<0) m-=l;
			else m+=l;
		}
	}
	
	*this=m;

	return *this;		
}

decimal decimal::operator/(decimal &d) {
	decimal temp(*this);
	return (temp/=d);
}

decimal& decimal::ShiftLeft() {
	int carry=0;
	for(int i=_D_SIZE_-1;i>=0;i--) {
		number[i]<<=1;
		number[i]+=carry;
		carry=(number[i]>=1000000000)?1:0;
		if(carry) number[i]%=1000000000;
	}
	return *this;
}

decimal& decimal::ShiftRight() {
	int carry=0, temp=0;
	for(int i=0;i<_D_SIZE_;i++) {
		temp=number[i]&1;
		number[i]>>=1;
		number[i]+=carry;
		carry=(temp)?500000000:0;
	}		
	return *this;
}

decimal& decimal::operator<<=(int n) {
	for(int i=0;i<n;i++) ShiftLeft();
	return *this;
}

decimal decimal::operator<<(int n) {
	decimal temp(*this);
	return (temp<<=n);
}

decimal& decimal::operator>>=(int n) {
	for(int i=0;i<n;i++) ShiftRight();
	return *this;
}

decimal decimal::operator>>(int n) {
	decimal temp(*this);
	return (temp>>=n);
}

decimal& decimal::operator^=(int n) {
	decimal temp(*this);
	for(int i=1;i<n;i++) *this*=temp;
	return *this;
}

decimal decimal::operator^(int n) {
	decimal temp(*this);
	return (temp^=n);
}

 bool decimal::isnull() const {
	return (number[0]==1111111111);
}
	
decimal& decimal::Absolute() {
	sign=1;
	return *this;
}	

decimal& decimal::Floor() {
	int32 temp=0;
	for(int i=_D_SIZE_-_D_PREC_;i<_D_SIZE_;i++) { temp|=number[i]; number[i]=0; }
	if(sign>=0) return *this;
	if(temp>0) --*this;
	return *this;
}

decimal& decimal::Ceil() {
	int32 temp=0;
	for(int i=_D_SIZE_-_D_PREC_;i<_D_SIZE_;i++) { temp|=number[i]; number[i]=0; }
	if(sign<0) return *this;
	if(temp>0) ++*this;
	return *this;
}

bool decimal::IsNullInstance() const { return isnull(); }

decimal::operator Value() const 		{ return RichToValue(*this); }

decimal::decimal(const Value& v, int precision, int rounding) 	{ FromString(AsString(v)); trim_trailing_zeroes=true; print_precision=precision; rounding_mode=rounding; }	
	
int decimal::Compare(const Value& b) const { return CompareTo(b); }

void decimal::Serialize(Stream &s)		{ s.Put(ToString()); }

unsigned decimal::GetHashValue()	const	{ 
	return ::GetHashValue(ToString());;
}

decimal abs(decimal d) {
	return d.Absolute();
}

decimal floor(decimal d) {
	return d.Floor();
}

decimal ceil(decimal d) {
	return d.Ceil();
}

decimal sqrt(decimal d) {
	return d.Sqrt();
}

decimal round(decimal d, int precision, int rounding) {
	decimal temp(d);
	return temp.Round(precision, rounding);
}

static decimal operator+(Value t, decimal d) {
	decimal temp(t);
	return (temp+=d);
}

static decimal operator-(Value t, decimal d) {
	decimal temp(t);
	return (temp-=d);
}

static decimal operator*(Value t, decimal d) {
	decimal temp(t);
	return (temp*=d);
}

static decimal operator/(Value t, decimal d) {
	decimal temp(t);
	return (temp/=d);
}

decimal decimal::ZERO=decimal(0);
decimal decimal::ONE=decimal(1);
decimal decimal::TWO=decimal(2);
decimal decimal::TEN=decimal(10);
