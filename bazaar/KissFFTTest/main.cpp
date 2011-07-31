#include "KissFFTTest.h"

#define N 8

#define CD 1

void print_buffs(const kiss_fft_cpx* sin, const kiss_fft_cpx* sout, int n)
{
	char s[2048];
	for(int i=0; i<n; i++)
	{
		sprintf(s, "sin[%u]: r = %lf i = %lf sout: r = %lf i = %lf",i,sin[i].r,sin[i].i,sout[i].r,sout[i].i);
		RLOG(s);
	}
}

KissFFTTest::KissFFTTest()
{
	CtrlLayout(*this, "Window title");
	
#if _DIRECT_KISSFFT_MEANS

	kiss_fft_cpx sin[N];
	kiss_fft_cpx sout[N];
	
	for(int i=0; i<N; i++)
	{
		sin[i].r = (kiss_fft_scalar)0;
		sin[i].i = (kiss_fft_scalar)0;
	}
		sin[0].r = (kiss_fft_scalar)1;


	kiss_fft_cfg st = kiss_fft_alloc(N,0,0,0);
	kiss_fft(st,sin,sout);
	kiss_fft_free(st);

	print_buffs(sin, sout, N);

	RLOG("");

	st = kiss_fft_alloc(N,1,0,0);
	kiss_fft(st,sout,sin);
	kiss_fft_free(st);

	//apply scale
	double sc = 1./N;
	for(int i = 0; i < N; i++)
	{
		sin[i].r *= sc, sin[i].i *= sc;
	}
	print_buffs(sout, sin, N);

#else

	//USING UPP STRUCTURES

#ifdef CD
	Vector<Complex> vt, vf;
#else
	Vector<kiss_fft_cpx> vt, vf;
#endif

	vt.SetCount(N); //in
	vf.SetCount(N); //out

#ifdef CD
	vt[0] = Complex(1.);
#else
	for(int i=0; i<N; i++)
	{ vt[i].r = (kiss_fft_scalar)0, vt[i].i = (kiss_fft_scalar)0; }
	vt[0].r = (kiss_fft_scalar)1;
#endif

	RLOG("fw fft");
	{
		KissFFT fft(N);
		fft(vf, vt);
	}
#ifdef CD
	DUMPC(vt);
	DUMPC(vf);
#else
	print_buffs((const kiss_fft_cpx*)vt, (const kiss_fft_cpx*)vf, N);
#endif
	
	RLOG("bw fft");
	{
		KissFFT fft(N,true);
		fft(vt, vf);
	}
	double sc = 1./N;
#ifdef CD
	for(int i = 0; i < vt.GetCount(); i++)
		vt[i] *= sc;
	DUMPC(vf);
	DUMPC(vt);
#else
	for(int i = 0; i < vt.GetCount(); i++)
	{ vt[i].r *= sc, vt[i].i *= sc; }
	print_buffs((const kiss_fft_cpx*)vf, (const kiss_fft_cpx*)vt, N);
#endif

#endif
}

GUI_APP_MAIN
{
	KissFFTTest().Run();
}

