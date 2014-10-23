#include "Entropy.h"

String FChar(int c)
{
	String s = Format("0x%02x", c) + " ";
	if(c >= ' ' && c < 128)
		return s + String(c, 1);
	return s + ".";
}

