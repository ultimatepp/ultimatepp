#if defined(classA)
	#if defined(classP)
		#define classAP classA, classP
		#define classAPc , classA, classP
		#define AMP_     A_, MP_
		#define ap_      a_, p_
	#else
		#define classAP classA
		#define classAPc , classA
		#define AMP_     A_
		#define ap_      a_
	#endif
#elif defined(classP)
	#define classAP classP
	#define classAPc , classP
	#define AMP_     MP_
	#define ap_      p_
#else
	#define classAP
	#define classAPc
	#define AMP_
	#define ap_
#endif

#if defined(classA) || defined(classP)
template <classAP>
#else
inline
#endif
Cb_<A_> callbackn(Res (*fn)(AMP_) P_p_)
{
	return Cb_<A_>([=](A_a_) { return (*fn)(ap_); }, 0);
}

template <class O, class M classAPc>
Cb_<A_> callbackn(O *object, Res (M::*method)(AMP_) P_p_)
{
	return Cb_<A_>([=](A_a_) { return (object->*method)(ap_); }, 0);
}

template <class O, class M classAPc>
Cb_<A_> callbackn(O *object, Res (M::*method)(AMP_) const P_p_)
{
	return Cb_<A_>([=](A_a_) { return (object->*method)(ap_); }, 0);
}

template <class O, class M classAPc>
Cb_<A_> ptebackn(O *object, Res (M::*method)(AMP_) P_p_)
{
	Ptr<O> ptr = object;
	return Cb_<A_>([=](A_a_) { if(ptr) return (ptr->*method)(ap_); return Res(); }, 0);
}

template <class O, class M classAPc>
Cb_<A_> ptebackn(O *object, Res (M::*method)(AMP_) const P_p_)
{
	Ptr<O> ptr = object;
	return Cb_<A_>([=](A_a_) { if(ptr) return (object->*method)(ap_); return Res(); }, 0);
}

#ifdef classP
template <classAP>
Cb_<A_> callbackn(Cb_<AMP_> cb P_p_)
{
	return Cb_<A_>([=](A_a_) { return cb(ap_); }, 0);
}
#endif

#undef classAP
#undef classAPc
#undef AMP_
#undef ap_

#undef callbackn
#undef ptebackn
#ifdef classP
	#undef classP
#endif
#undef P_
#undef P_p_
#undef p_
#undef MP_