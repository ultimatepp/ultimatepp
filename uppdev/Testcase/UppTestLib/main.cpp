#include <QtGui/QtGui>
#include <QtGui/QStyle>
#include <X11/X.h>
#include <iostream>

// #define LOG(str)
#define LOG(str) std::clog << str << std::endl

#if defined(__cplusplus)
extern "C" {
#endif

static bool initted = false;
QApplication *app = 0;

int Init()
{
	if (!initted) {
		int argc = 1;
		char *argv[] = {"qapp", NULL};
		if (!QApplication::instance()) {
			app = new QApplication(argc, argv);
			if (app) {
				initted = true;
				LOG("Init()");
				return true;
			}
		} else {
			initted = true;
			return true;
		}
		return false;
	}
}

int IsInitted()
{
	LOG("IsInitted()");
	return initted;
}

void UnInit()
{
	if (initted) {
		if (app) {
			delete app;
			app = 0;
		}
		initted = false;
	}
	LOG("UnInit()");
}

#if defined(__cplusplus)
}
#endif
