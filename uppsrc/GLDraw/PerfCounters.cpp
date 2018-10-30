#include "GLDraw.h"

namespace Upp {

int sDrawCounter;
int sElementCounter;
int sTextureCounter;
int sProgramCounter;

void GLClearCounters()
{
	sElementCounter = sTextureCounter = sProgramCounter = sDrawCounter = 0;
}

int GLElementCounter()
{
	return sElementCounter;
}

int GLTextureCounter()
{
	return sTextureCounter;
}

int GLProgramCounter()
{
	return sProgramCounter;
}

int GLDrawCounter()
{
	return sDrawCounter;
}

};