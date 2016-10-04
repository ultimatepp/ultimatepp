#include <stdio.h>
#include <process.h>
#include <windows.h>
#include <vector>
#include <Core/Core.h>
using namespace Upp;

int t0=GetTickCount();
void t() { printf("%.2f\t",(GetTickCount()-t0)*1e-3); }
std::vector<int> *v;
Mutex mutex;

unsigned __stdcall th1(void*ctx) {
	int i=(int)ctx;
	t();printf("thread%d start\n",i);
	Sleep(100);
	{ Mutex::Lock lock(mutex);
		t();printf("clear & add item\n");
		v->clear();
		v->push_back(10+i);
	}
	Sleep(100);
	t();printf("thread%d done\n",i);
	return 0;
}

void th0(int k) {
	v = new std::vector<int>();
	t();printf("born\n");
	for(int i=1;i<k;++i) {
		Sleep(100);
		int h=(int)_beginthreadex(0,0,th1,(void*)i,CREATE_SUSPENDED,0);
		t(); printf("create thread %d\n",h);
		Sleep(100);
		{ Mutex::Lock lock(mutex);
			v->clear();
		}
		t();printf("resume thread\n");
		ResumeThread((HANDLE)h);
		Sleep(300);
		{ Mutex::Lock lock(mutex);
			int n=v->size();
			t(); printf("n=%d ""x[n-1]=%d\n",n,n?(*v)[n-1]:-1);
		}
	}
	Sleep(100);
	{ Mutex::Lock lock(mutex);
		t();printf("clear\n");
		v->clear();	
	}
	t();printf("kill\n");
	delete v;
}

CONSOLE_APP_MAIN{	
	th0(4);
	printf("ready!\n");
}
