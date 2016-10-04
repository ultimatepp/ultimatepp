/***********************************************************************************************
	
	Ultimate++ Multi Threading example
	
	Copyright (c) 2007, Artur Bac
	 ______ ____           _____        __ _   
	|  ____|  _ \   /\    / ____|      / _| |  
	| |__  | |_) | /  \  | (___   ___ | |_| |_ 
	|  __| |  _ < / /\ \  \___ \ / _ \|  _| __|
	| |____| |_) / ____ \ ____) | (_) | | | |_ 
	|______|____/_/    \_\_____/ \___/|_|  \__|

	All rights reserved.

	Redistribution and use in source and binary forms, with or without modification, 
	are permitted provided that the following conditions are met:

    	* Redistributions of source code must retain the above copyright notice, 
    	  this list of conditions and the following disclaimer.
    	* Redistributions in binary form must reproduce the above copyright notice, 
    	  this list of conditions and the following disclaimer in the documentation 
    	  and/or other materials provided with the distribution.
    	* Neither the name of the EBASoft nor the names of its contributors may be used 
    	  to endorse or promote products derived from this software without specific 
    	  prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
	CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
	EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
	PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
	LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***********************************************************************************************/

#ifndef _Threads_h
#define _Threads_h

#include <CtrlLib/CtrlLib.h>
#include "paintbox.h"
#include "complex.h"

using namespace Upp;

#define LAYOUTFILE <Threads/Threads.lay>
#include <CtrlCore/lay.h>

class Threads : public WithThreadsLayout<TopWindow> 
{
public:
	typedef Threads CLASSNAME;
	typedef Callback2<const Complex &, int *> FractalJobHandeler;
	
	Threads();
	
	void 						picbox_Paint( Upp::Draw & g );	//draw area paint rutine
	void 						picbox_LeftDown( Point p, dword keyflags );
	void 						picbox_LeftUp( Point p, dword keyflags );
	void 						picbox_MouseMove( Point p, dword keyflags );
	
private:
	StatusBar 					statusBar;
	InfoCtrl    				time;
	int 						densityR, densityC, maxIter;
	int							rows, cols;
	int							max_threads;
	ArrayMap<int, Image>		rendered;
	VectorMap<int,int> 			active;							//Active map
	volatile Atomic 			threads;						//thread counter
	Mutex						job_lock;						//lock used for accesing active map
	Mutex						image_lock;						//lock used for accesing active map
	Semaphore 					waitforfinish;					//semaphore for signaling thread finish	
	Rect_<double>				area;					
	Complex						julia_c;
	Point 						posOnClick;
	Size 						moveShift;
	bool 						moving;
	double						logmax;
	
	void						Process(const FractalJobHandeler & job);						
	void						DoJob();
	void						ThreadRenderJob(int r, int c, const FractalJobHandeler & job, int);
	void						DrawAxis(Upp::Draw & g);
	void						SetSelected();
	void 						Refresh() const;
	//Fractal jobs
	void						FnMandelbrot(const Complex & cpx, int *res) const;
	void 						FnQubicMandelbrot(const Complex & cpx, int *res) const;
	void 						FnQuadricMandelbrot(const Complex & cpx, int *res) const;
	void 						FnJulia(const Complex & cpx, int *res) const;
	void 						FnQubicJulia(const Complex & cpx, int *res) const;
	void 						FnQuadricJulia(const Complex & cpx, int *res) const;
	void 						FnPhoenix(const Complex & cpx, int *res) const;
	void 						FnBacPhoenix(const Complex & cpx, int *res) const;
	void 						FnBac(const Complex & cpx, int *res) const;
};

#endif

