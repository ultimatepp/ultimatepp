#ifndef assist_parser__

#include SHADER

#ifdef CREATE_INFO
	CREATE_INFO.pCode = pCode;
	CREATE_INFO.codeSize = codeSize;
	#undef CREATE_INFO
#else
	createInfo.pCode = pCode;
	createInfo.codeSize = codeSize;
#endif

#undef SHADER

#endif
