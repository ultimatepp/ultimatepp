#ifndef _PyTest_PyTest_h
#define _PyTest_PyTest_h

//must go first cause has some preprocessor things, says docu
#include <Python/Python.h>

#include <Core/Core.h>

using namespace Upp;

void SimpleCall();
void EmbedPython();
void ExtendPython();

void DestroyNewModule();

#endif

