#include <Core/Core.h>

using namespace Upp;

#include "Dim2.h"
#include "Dim3.h"
#include "Dim4.h"
#include "Dim5.h"
#include "Dim.hpp"

int gi;
int hi;

void Foo() {
	gi++;
}

Dim2<double> Box(int n) {
	RLOG("---");
	Dim2<double> d2;
	d2.Create(n, n);
	for(int pass = 0; pass < 2; pass++) {
		int q = 0;
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
					if(pass)
						ASSERT(d2[i][j] == ++q);
					else
						d2[i][j] = ++q;
	}
	return d2;
}


Dim3<double> Cube(int n) {
	RLOG("---");
	Dim3<double> d3;
	d3.Create(n, n, n);
	for(int pass = 0; pass < 2; pass++) {
		int q = 0;
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
				for(int k = 0; k < n; k++)
					if(pass)
						ASSERT(d3[i][k][j] == ++q);
					else
						d3[i][k][j] = ++q;
	}
	return d3;
}

Dim4<double> Qube(int n) {
	RLOG("---");
	Dim4<double> d4;
	d4.Create(n, n, n, n);
	for(int pass = 0; pass < 2; pass++) {
		int q = 0;
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
				for(int k = 0; k < n; k++)
					for(int l = 0; l < n; l++)
						if(pass)
							ASSERT(d4[i][k][j][l] == ++q);
						else
							d4[i][k][j][l] = ++q;
	}
	return d4;
}

Dim5<double> Cube5(int n) {
	RLOG("---");
	Dim5<double> d5;
	d5.Create(n, n, n, n, n);
	for(int pass = 0; pass < 2; pass++) {
		int q = 0;
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
				for(int k = 0; k < n; k++)
					for(int l = 0; l < n; l++)
						for(int m = 0; m < n; m++)
							if(pass)
								ASSERT(d5[i][k][j][l][m] == ++q);
							else
								d5[i][k][j][l][m] = ++q;
	}
	return d5;
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
	for(int i = 0; i < 20; i++) {
		RLOG(i);
		Box(i);
	}
	for(int i = 0; i < 20; i++) {
		RLOG(i);
		Cube(i);
	}
	for(int i = 0; i < 20; i++) {
		RLOG(i);
		Qube(i);
	}
	for(int i = 0; i < 20; i++) {
		RLOG(i);
		Cube5(i);
	}
	
	Dim2<double> b = Box(20);
	RDUMP(b[19][19]);
	
	Dim3<double> x = Cube(20);
	RDUMP(x[0][0][0]);
	RLOG("---");
	x = Cube(40);
	RDUMP(x[10][10][10]);
	
	const Dim3<double>& y = x;
	RDUMP(y[10][10][10]);
	
	Dim4<double> qx = Qube(20);
	RDUMP(qx[19][19][19][19]);
	const Dim4<double>& qy = qx;
	RDUMP(qy[10][10][10][10]);

	Dim5<double> fx = Cube5(10);
	RDUMP(fx[9][9][9][9][9]);
	const Dim5<double>& fy = fx;
	RDUMP(fy[9][9][9][9][9]);
}
