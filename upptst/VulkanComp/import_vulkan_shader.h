#ifndef assist_parser__

#define  SHADER_FN <SHADER.i>
#include SHADER_FN

#ifdef CREATE_INFO
	CREATE_INFO.pCode = pCode;
	CREATE_INFO.codeSize = codeSize;
#else
	createInfo.pCode = pCode;
	createInfo.codeSize = codeSize;
#endif

#undef SHADER_FN
#undef SHADER

#endif
