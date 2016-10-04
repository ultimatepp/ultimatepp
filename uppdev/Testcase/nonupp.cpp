#include <iostream>
#include <string>
#include <X11/Xlib.h>
#include <dlfcn.h>

typedef int (*Init_t)(Display *);
typedef int (*IsInitted_t)();
typedef void (*UnInit_t)();

int main(){
	
	void *camo = dlopen("./libtest.so", RTLD_LAZY);
	if (!camo) {
		std::cerr << dlerror() << std::endl;
		return 1;
	}
	
	Init_t camo_init = (Init_t) dlsym(camo, "Init");
	if (!camo_init) {
		std::cerr << dlerror() << std::endl;
		return 1;
	}
	IsInitted_t camo_isinitted = (IsInitted_t) dlsym(camo, "IsInitted");
	if (!camo_isinitted) {
		std::cerr << dlerror() << std::endl;
		return 1;
	}
	UnInit_t camo_uninit = (UnInit_t) dlsym(camo, "UnInit");
	if (!camo_uninit) {
		std::cerr << dlerror() << std::endl;
		return 1;
	}

	Display *d;
	Window w;
	XEvent e;
	int s;
	 
	d = XOpenDisplay(NULL);
	if (d == NULL) {
		std::cerr << "Cannot open display\n";
		return 0;
	}
   	
	if (!camo_isinitted()) {
		camo_init(d);
	}

	s = DefaultScreen(d);
	w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 100, 100, 1,
			BlackPixel(d, s), WhitePixel(d, s));
	XSelectInput(d, w, KeyPressMask);
	XMapWindow(d, w);
	Atom wmDelete=XInternAtom(d, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(d, w, &wmDelete, 1);
	while (1) {
		XNextEvent(d, &e);
		if (e.type == ClientMessage)
			break;
		if (e.type == KeyPress)
			break;
	}
	
	if (camo_isinitted()) {
		camo_uninit();
	}
	
	if (camo)
		dlclose(camo);
	else
		std::clog << dlerror() << std::endl;
		
	XCloseDisplay(d);
	return 0;
}

