#ifndef _GridCtrl_GridUtils_h_
#define _GridCtrl_GridUtils_h_

inline int32 Round(double a)
{
	#ifdef flagGCC
	return (int) a;
	#else
	int32 retval;

	__asm fld a
	__asm fistp retval
	return retval;

	#endif
}

//inline int32 Round(double a);

#endif
