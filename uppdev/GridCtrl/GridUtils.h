#ifndef _GridCtrl_GridUtils_h_
#define _GridCtrl_GridUtils_h_

inline int32 Round(double a)
{
	#ifdef flagLINUX
	return (int) a;
	#endif
	#ifdef flagWIN32
	int32 retval;

	__asm fld a
	__asm fistp retval
	return retval;

	#endif
}

//inline int32 Round(double a);

#endif
