#include <CtrlLib/CtrlLib.h>
#include "Shaders.h"

NAMESPACE_UPP

void printShaderInfoLog(GLuint obj, const char* fileName)
{
	int infoLength = 0;
	int charsWritten = 0;

	DeleteFile(fileName);
	
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infoLength);

	if(infoLength > 1)
	{
		char* info = new char[infoLength];
		glGetShaderInfoLog(obj, infoLength, &charsWritten, info);
		if(charsWritten > 0)
		{
			FileOut f(fileName);
			f.Put(info);
		}
		delete[] info;
	}
}

void printProgramInfoLog(GLuint obj, const char* fileName)
{
	int infoLength = 0;
	int charsWritten = 0;

	DeleteFile(fileName);
	
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infoLength);

	if(infoLength > 1)
	{
		char* info = new char[infoLength];
		glGetShaderInfoLog(obj, infoLength, &charsWritten, info);
		if(charsWritten > 0)
		{
			FileOut f(fileName);
			f.Put(info);
		}
		delete[] info;
	}
}

void printProgramValidationLog(GLuint obj, const char* fileName)
{
	char log[1024];
	int len = 0;
	glValidateProgram(obj);
	glGetProgramInfoLog(obj, 1024, &len, log);
	if(len > 0)
	{
		FileOut f(fileName);
		f.Put(log);
	}
}

bool checkGLError()
{
	unsigned int lastError = glGetError();
	if(GL_NO_ERROR != lastError)
	{
		switch(lastError) 
		{
			case GL_INVALID_ENUM:
				RLOG("GL_INVALID_ENUM");
				break;
			case GL_INVALID_VALUE:
				RLOG("GL_INVALID_VALUE");
				break;
			case GL_INVALID_OPERATION:
				RLOG("GL_INVALID_OPERATION");
				break;
			case GL_STACK_OVERFLOW:
				RLOG("GL_STACK_OVERFLOW");
				break;
			case GL_STACK_UNDERFLOW:
				RLOG("GL_STACK_UNDERFLOW");
				break;
			case GL_OUT_OF_MEMORY:
				RLOG("GL_OUT_OF_MEMORY");
				break;
		}
		return false;
	}
	return true;
}

int CompileProgram(const char* frag, const char* vert)
{
	const GLchar* vertexShaderSrc = (const GLchar*) vert;
	const GLchar* fragmentShaderSrc = (const GLchar*) frag;
	
	int program = glCreateProgram();

	GLenum vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLenum fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	if(!checkGLError())
		return -1;
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
	if(!checkGLError())
		return -2;
	glCompileShader(vertexShader);
	if(!checkGLError())
		return -3;
	glCompileShader(fragmentShader);
	if(!checkGLError())
		return -4;
	glAttachShader(program, vertexShader);
	if(!checkGLError())
		return -5;
	glAttachShader(program, fragmentShader);
	if(!checkGLError())
		return -6;
	glLinkProgram(program);
	if(!checkGLError())
		return -7;

	printShaderInfoLog(vertexShader, "vertex.log");
	printShaderInfoLog(fragmentShader, "fragment.log");
	printProgramInfoLog(program, "program.log");
	printProgramValidationLog(program, "programv.log");
	
	return program;
}

const String fragAlphaMag = "\
uniform sampler2D textMap; \
uniform vec4 textColor; \
\
float alphaThreshold = 0.5;\
float distanceScale = 15.0;\
\
float outlineOuter = 0.3;\
float outlineInner = 0.1;\
vec4 outlineColor = vec4(0, 0, 0, 1);\
\
float glowOffsetX = 0.005;\
float glowOffsetY = 0.005;\
float glowStart = .7;\
float glowEnd = 0.5;\
vec4 glowColor = vec4(0.0, 0.0, 0.0, 1.0);\
\
void main()\
{\
	vec4 color = texture2D(textMap, gl_TexCoord[0].xy);\
	float distanceFactor = color.a;\
	\
	float width = fwidth(gl_TexCoord[0].xy) * distanceScale;\
	\
	/*float outlineMin1 = outlineOuter + width * 2.0;\
	float outlineMax1 = outlineInner + width * 2.0;\
	\
	if (distanceFactor > outlineOuter && distanceFactor < outlineMax1)\
	{\
		float outlineAlpha;\
		\
		if (distanceFactor <= outlineMin1)\
			outlineAlpha = smoothstep(outlineOuter, outlineMin1, distanceFactor);\
		else\
			outlineAlpha = smoothstep(outlineMax1, outlineInner, distanceFactor);\
			\
		color = mix(color, outlineColor, outlineAlpha);\
	}*/\
	\
	color.rgb = textColor.rgb; \
	color.a = smoothstep(alphaThreshold - width, alphaThreshold + width, distanceFactor);\
	\
	/*vec2 glowOffset = vec2(-glowOffsetX, -glowOffsetY);\
	float glowDistance = texture2D(textMap, gl_TexCoord[0].xy + glowOffset).a;\
	float glowFactor = smoothstep(glowStart, glowEnd, glowDistance);\
	\
	color = mix(vec4(glowColor.rgb, glowFactor), color, color.a);*/\
	\
	gl_FragColor = color;\
}\
";
const String vertAlphaMag = "\
void main() \
{ \
	gl_TexCoord[0] = gl_MultiTexCoord0; \
	gl_Position = ftransform(); \
} \
";

END_UPP_NAMESPACE