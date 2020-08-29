#ifndef _SurfaceCtrl_Shader_h_
#define _SurfaceCtrl_Shader_h_
#include "Definition.h"
//#include <GLCtrl_glad/GLCtrl_glad.h>
namespace Upp{
class OpenGLShader{
	private:
		GLenum shaderType;
		GLuint ID = 0;
		bool compiled = false;
		
		bool CheckForCompilationError()noexcept{
			int success;
		    char infoLog[512];
		    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
		    if (!success){
		        glGetShaderInfoLog(ID, 512, NULL, infoLog);
		        LOG("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + String(infoLog));
				return false;
		    }
			return true;
		}
	public:
		OpenGLShader(GLenum type,const char * data) : shaderType{type} {
			ID = glCreateShader(shaderType);
		    glShaderSource(ID, 1, &data, NULL);
		    glCompileShader(ID);
		    if(CheckForCompilationError())compiled = true;
		}
		~OpenGLShader(){glDeleteShader(ID);}
		
		GLuint GetID()const noexcept{return ID;}
		GLenum GetType()const noexcept{return shaderType;}
		bool IsCompiled()const noexcept{return compiled;}
};
	class OpenGLProgram{
	private:
		GLuint vertex = 0;
		GLuint TCS = 0; //Tesselation control shader
		GLuint TES = 0; //Tesselation evaluation shader
		GLuint geometry = 0;
		GLuint fragment = 0;
		
		GLuint ID = 0;
		bool linked = false;
		bool copied = false; //if the shader is copied then is deletion wont occure a clear in OpenGL data
		
		GLint GetUniformLocation(Upp::String name){
			GLint location = glGetUniformLocation(ID, name.ToStd().c_str());
			if(location == -1)RLOG("Warning : uniform named " + name +" can't be find in program number " + AsString(ID));
			return location;
		}
		
		bool CheckForLinkingError()noexcept{
			int success;
		    char infoLog[512];
			glGetProgramiv(ID, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(ID, 512, NULL, infoLog);
				LOG("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + String(infoLog));
				return false;
			}
			return true;
		}
	public:
		OpenGLProgram(){}
		OpenGLProgram(const OpenGLProgram& prog){*this = prog;}
		OpenGLProgram& operator=(const OpenGLProgram& prog)noexcept{
			vertex = prog.vertex;
			TCS = prog.TCS;
			TES = prog.TES;
			geometry = prog.geometry;
			fragment = prog.fragment;
			ID = prog.ID;
			linked = prog.linked;
			copied = true;
			return *this;
		}
		
		~OpenGLProgram(){if(ID != 0 && !copied) glDeleteProgram(ID);}
		
		OpenGLProgram& Bind()noexcept{if(linked)glUseProgram(ID);return *this;}
		OpenGLProgram& UnBind()noexcept{glUseProgram(0);return *this;}
		
		bool ContainVertex()const noexcept{return (linked)? vertex : false;}
		bool ContainTCS()const noexcept{return (linked)? TCS : false;}
		bool ContainTES()const noexcept{return (linked)? TES : false;}
		bool ContainGeometry()const noexcept{return (linked)? geometry : false;}
		bool ContainFragment()const noexcept{return (linked)? fragment : false;}
		
		OpenGLProgram& AttachShader(const OpenGLShader& shad)noexcept{
			if(shad.IsCompiled()){
				switch(shad.GetType()){
					case GL_VERTEX_SHADER:
						vertex = shad.GetID();
						break;
					case GL_TESS_CONTROL_SHADER:
						TCS = shad.GetID();
						break;
					case GL_TESS_EVALUATION_SHADER:
						TES = shad.GetID();
						break;
					case GL_GEOMETRY_SHADER:
						geometry = shad.GetID();
						break;
					case GL_FRAGMENT_SHADER:
						fragment = shad.GetID();
						break;
					default:
						break;//Unknow shader type no added at all
				}
			}
			return *this;
		}
		bool Link()noexcept{
			ID = glCreateProgram();
			if(vertex != 0) glAttachShader(ID,vertex);
			if(TCS != 0) glAttachShader(ID,TCS);
			if(TES != 0) glAttachShader(ID,TES);
			if(geometry != 0) glAttachShader(ID,geometry);
			if(fragment != 0) glAttachShader(ID,fragment);
			glLinkProgram(ID);
			linked = CheckForLinkingError();
			return linked;
		}
		Upp::VectorMap<GLenum,GLuint> GetProgramShaders()const noexcept{//Return all active shader in this program, Vector map contain Type of shader as key and ID of shader as value
			Upp::VectorMap<GLenum,GLuint> buffer;
			if(vertex != 0) buffer.Add(GL_VERTEX_SHADER,vertex);
			if(TCS != 0) buffer.Add(GL_TESS_CONTROL_SHADER,TCS);
			if(TES != 0) buffer.Add(GL_TESS_EVALUATION_SHADER,TES);
			if(geometry != 0) buffer.Add(GL_GEOMETRY_SHADER,geometry);
			if(fragment != 0) buffer.Add(GL_FRAGMENT_SHADER,fragment);
			return buffer;
		}
		GLuint GetID()const noexcept{return ID;}
		bool IsLinked()const noexcept{return linked;}
		
		OpenGLProgram& SetBool(Upp::String name, bool value)noexcept{if(linked)glUniform1i(GetUniformLocation(name), (int)value);return *this;}
		OpenGLProgram& SetInt(Upp::String name, int value)noexcept{if(linked)glUniform1i(GetUniformLocation(name), value);return *this;}
		OpenGLProgram& SetFloat(Upp::String name, float value)noexcept{if(linked)glUniform1f(GetUniformLocation(name), value);return *this;}
		OpenGLProgram& SetVec2(Upp::String name, const glm::vec2 &value)noexcept{if(linked)glUniform2fv(GetUniformLocation(name), 1, &value[0]);return *this;}
		OpenGLProgram& SetVec2(Upp::String name, float x, float y)noexcept{if(linked)glUniform2f(GetUniformLocation(name), x, y);return *this;}
		OpenGLProgram& SetVec3(Upp::String name, const glm::vec3 &value)noexcept{if(linked)glUniform3fv(GetUniformLocation(name), 1, &value[0]);return *this;}
		OpenGLProgram& SetVec3(Upp::String name, float x, float y, float z)noexcept{if(linked)glUniform3f(GetUniformLocation(name), x, y, z);return *this;}
		OpenGLProgram& SetVec4(Upp::String name, const glm::vec4 &value)noexcept{if(linked)glUniform4fv(GetUniformLocation(name), 1, &value[0]);return *this;}
		OpenGLProgram& SetVec4(Upp::String name, float x, float y, float z, float w)noexcept{if(linked)glUniform4f(GetUniformLocation(name), x, y, z, w);return *this;}
		OpenGLProgram& SetMat2(Upp::String name, const glm::mat2 &mat)noexcept{if(linked)glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);return *this;}
		OpenGLProgram& SetMat3(Upp::String name, const glm::mat3 &mat)noexcept{if(linked)glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);return *this;}
		OpenGLProgram& SetMat4(Upp::String name, const glm::mat4 &mat)noexcept{if(linked)glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);return *this;}
	};
}


#endif
