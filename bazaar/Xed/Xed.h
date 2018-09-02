#ifndef _Xed_Xed_h_
#define _Xed_Xed_h_

#include <Core/Core.h>

#define XED __getXED()

class XEDClass
{
	friend XEDClass &__getXED(void);
	
	private:
		
		// constructor
		XEDClass();
		
		// 64 bit mode
		bool _64bitMode;
		
	protected:
		
	public:

		// destructor
		~XEDClass();
		
		// set/get 64 bit mode
		void Set64bitMode(void);
		void Set32bitMode(void);
		bool Get64bitMode(void) const;
		bool Get32bitMode(void) const;
		
		// get instruction length from pointer
		int InstructionLength(void const *buf);
		
		// disassemble instruction at pointer
		Upp::String DisassembleInstruction(void const *buf, bool bytes = false);

		// dump all instruction info
		Upp::String DumpInstruction(void const *buf);
};

XEDClass &__getXED(void);



#endif
