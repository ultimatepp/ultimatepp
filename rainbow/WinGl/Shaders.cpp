#include <CtrlLib/CtrlLib.h>
#include "Shaders.h"

NAMESPACE_UPP

void printShaderInfoLog(GLuint obj, const char* fileName)
{
	int infoLength = 0;
	int charsWritten = 0;

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
		error = (const char*) gluErrorString(lastError);
		return false;
	}
	return true;
}

bool IsProgramCompiled(int program)
{
	GLint result = GL_FALSE;
	glGetShaderiv(program, GL_COMPILE_STATUS, &result);
	return result == GL_TRUE;
}

int CompileProgram(const char* frag, const char* vert)
{
	const GLchar* vertexShaderSrc = (const GLchar*) vert;
	const GLchar* fragmentShaderSrc = (const GLchar*) frag;
	
	DeleteFile("shader_vertex.log");
	DeleteFile("shader_fragment.log");
	DeleteFile("shader_link.log");
	DeleteFile("shader_validation.log");
	
	int program = glCreateProgram();

	GLenum vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLenum fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShader);
	if(!IsProgramCompiled(vertexShader))
	{
		error = "Vertex shader compilation error (see shader_vertex.log)";
		printShaderInfoLog(vertexShader, "shader_vertex.log");
		return -3;
	}
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShader);
	if(!IsProgramCompiled(fragmentShader))
	{
		error = "Fragment shader compilation error (see shader_fragment.log)";
		printShaderInfoLog(fragmentShader, "shader_fragment.log");
		return -4;
	}
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	//if(!IsProgramCompiled(program)) /* ATI always reports validation warning */
	{
		printProgramInfoLog(program, "shader_link.log");
		printProgramValidationLog(program, "shader_validation.log");
		//return -7;
	}
	
	return program;
}

const String fragAlphaMag =
"uniform sampler2D textMap;                                                                  \n"
"uniform vec4 textColor;                                                                     \n"
"                                                                                            \n"
"float alphaThreshold = 0.50;                                                                \n"
"float distanceScale = 15.0;                                                                 \n"
"                                                                                            \n"
"float outlineOuter = 0.3;                                                                   \n"
"float outlineInner = 0.1;                                                                   \n"
"vec4 outlineColor = vec4(0, 0, 0, 1);                                                       \n"
"                                                                                            \n"
"float glowOffsetX = 0.005;                                                                  \n"
"float glowOffsetY = 0.005;                                                                  \n"
"float glowStart = .7;                                                                       \n"
"float glowEnd = 0.5;                                                                        \n"
"vec4 glowColor = vec4(0.0, 0.0, 0.0, 1.0);                                                  \n"
"                                                                                            \n"
"void main()                                                                                 \n"
"{                                                                                           \n"
"	vec4 color = texture2D(textMap, gl_TexCoord[0].xy);                                      \n"
"	float distanceFactor = color.a;                                                          \n"
"	                                                                                         \n"
"	float width = fwidth(gl_TexCoord[0].x) * distanceScale;                                  \n"
"	                                                                                         \n"
"	/*float outlineMin1 = outlineOuter + width * 2.0;                                        \n"
"	float outlineMax1 = outlineInner + width * 2.0;                                          \n"
"	                                                                                         \n"
"	if (distanceFactor > outlineOuter && distanceFactor < outlineMax1)                       \n"
"	{                                                                                        \n"
"		float outlineAlpha;                                                                  \n"
"		                                                                                     \n"
"		if (distanceFactor <= outlineMin1)                                                   \n"
"			outlineAlpha = smoothstep(outlineOuter, outlineMin1, distanceFactor);            \n"
"		else                                                                                 \n"
"			outlineAlpha = smoothstep(outlineMax1, outlineInner, distanceFactor);            \n"
"			                                                                                 \n"
"		color = mix(color, outlineColor, outlineAlpha);                                      \n"
"	}*/                                                                                      \n"
"	                                                                                         \n"
"	color.rgb = textColor.rgb;                                                               \n"
"	color.a = smoothstep(alphaThreshold - width, alphaThreshold + width, distanceFactor);    \n"
"	                                                                                         \n"
"	/*vec2 glowOffset = vec2(-glowOffsetX, -glowOffsetY);                                    \n"
"	float glowDistance = texture2D(textMap, gl_TexCoord[0].xy + glowOffset).a;               \n"
"	float glowFactor = smoothstep(glowStart, glowEnd, glowDistance);                         \n"
"	                                                                                         \n"
"	color = mix(vec4(glowColor.rgb, glowFactor), color, color.a);*/                          \n"
"	                                                                                         \n"
"	gl_FragColor = color;                                                                    \n"
"}                                                                                           \n"
;


const String vertAlphaMag =
"void main()                                                                                 \n"
"{                                                                                           \n"
"	gl_TexCoord[0] = gl_MultiTexCoord0;                                                      \n"
"	gl_Position = ftransform();                                                              \n"
"}                                                                                           \n"
;
END_UPP_NAMESPACE