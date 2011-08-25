#ifndef _Media_mt_h_
#define _Media_mt_h_

// Classes thanks to U++ Forum supporters

// Safe atomic
class AtomicVar {
	private:
		Atomic val;
	public:
		AtomicVar() {};
		
		AtomicVar(const AtomicVar& p) {AtomicWrite(val, AtomicRead(p.val));}
		template <class T>
		AtomicVar& operator=(const T& p) {AtomicWrite(val, p); return *this;}

		operator int() {return AtomicRead(val);}
};

#endif
