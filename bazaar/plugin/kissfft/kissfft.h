#ifndef _plugin_kissfft_kissfft_h_
#define _plugin_kissfft_kissfft_h_

#include <Core/Core.h>
using namespace Upp;

#include "kiss_fft.h"
#include "kfc.h"
#include "kiss_fftnd.h"
#include "kiss_fftndr.h"
#include "kiss_fftr.h"

#include "kissfft.hh"

#include "kiss_fastfir.h"

NAMESPACE_UPP
NTL_MOVEABLE(kiss_fft_cpx);
END_UPP_NAMESPACE

//manages the plan
class KissFFT
{
public:
	KissFFT(int n = 512, bool inv = false) : n(n), st(NULL) { 
		ASSERT(sizeof(Complex) == sizeof(kiss_fft_cpx)); st = kiss_fft_alloc(n,inv?1:0,0,0); }
	~KissFFT() { kiss_fft_free(st); }

	void operator()(Vector<Complex>& out, const Vector<Complex>& in)
	{
		ASSERT(st); ASSERT(out.GetCount() == n); ASSERT(in.GetCount() == n);
		kiss_fft(st,(const kiss_fft_cpx*)(const Complex*)in,(kiss_fft_cpx*)(Complex*)out);
	}

	void operator()(Vector<kiss_fft_cpx>& out, const Vector<kiss_fft_cpx>& in)
	{
		ASSERT(st); ASSERT(out.GetCount() == n); ASSERT(in.GetCount() == n);
		kiss_fft(st,(const kiss_fft_cpx*)in,(kiss_fft_cpx*)out);
	}

protected:
	kiss_fft_cfg st;
	int n;
};

#endif
