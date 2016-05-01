#include <Core/Core.h>

using namespace Upp;

const int N = 1000000;

// M. D. MCILROY
// Appendix. An adversary for
// qsort

#include <stdlib.h>

int     *val;                            /* item values */
int     ncmp;                            /* number of comparisons */
int     nsolid;                          /* number of solid items */
int     candidate;                       /* pivot candidate */
int     gas;                             /* gas value */

#define freeze(x) val[x] = nsolid++
int cmp(const void *px, const void *py)  /* per C standard */
{
	const int x = *(const int*)px;
	const int y = *(const int*)py;
	ncmp++;
	if(val[x]==gas && val[y]==gas)
		if(x == candidate)
			freeze(x);
		else
			freeze(y);
	if(val[x] == gas)
		candidate = x;
	else if(val[y] == gas)
		candidate = y;
	return val[x] - val[y];          /* only the sign matters */
}

int antiqsort(int n, int *a)
{
	int i;
	int *ptr = (int *)malloc(n*sizeof(*ptr));
	val = a;
	gas = n - 1;
	nsolid = ncmp = candidate = 0;
	for(i=0; i<n; i++) {
		ptr[i] = i;
		val[i] = gas;
	}
	qsort(ptr, n, sizeof(*ptr), cmp);
	free(ptr);
	return ncmp;
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	for(int n = -1; n <= 10; n++) {
		Vector<String> h;
		for(int i = 0; i < N; i++)
			if(n < 0)
				h.Add(AsString(N - i));
			else
			if(n)
				h.Add(AsString(Random(n)));
			else
				h.Add(AsString(Random()));
		TimeStop tm;
		Sort(h);
		double t = tm.Seconds();
		RLOG(n << " " << t << " s");
	}
	
	Buffer<int> aq(N);
	RLOG("Generating antiqsort");
	antiqsort(N, aq);
	Vector<String> h;
	for(int i = 0; i < N; i++)
		h.Add(AsString(aq[i]));
	RLOG("Now sorting");
	TimeStop tm;
	Sort(h);
	double t = tm.Seconds();
	RLOG("antiqsort " << t << " s");
}
