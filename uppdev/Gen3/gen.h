#ifndef _Gen3_gen_h_
#define _Gen3_gen_h_

#include <Core/Core.h>

using namespace Upp;

#define COUNT 256

struct Circuit {
	struct Nand {
		word i1, i2;
	};
	
	Nand nand[COUNT];

	void Evaluate(byte *value) const;
	void SetRandom();
};


#endif
