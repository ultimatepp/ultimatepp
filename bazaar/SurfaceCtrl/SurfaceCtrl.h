#ifndef _UOGLCtrl_UOGLCtrl_h_
#define _UOGLCtrl_UOGLCtrl_h_
#include <CtrlLib/CtrlLib.h>
#include <GLCtrl_glad/GLCtrl_glad.h>
#include <Surface/Surface.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <plugin/glm/glm.hpp>
#include <plugin/glm/gtc/matrix_transform.hpp>
#include <plugin/glm/gtc/type_ptr.hpp>
#include <plugin/glm/gtx/quaternion.hpp>
#include <plugin/glm/ext/quaternion_trigonometric.hpp>
#include <plugin/glm/gtc/constants.hpp>
#include <plugin/glm/gtx/norm.hpp>
#include <plugin/glm/gtx/string_cast.hpp>

#define STRINGIFY(...) #__VA_ARGS__
#define SHADER(version, shader) "#version " #version "\n" STRINGIFY(shader)

#include "Object3D.h"
#include "Shader.h"
#include "TrackBallCamera.h"
#include "GLDrawFunction.h"

namespace Upp{
	
class SurfaceCtrl : public GLCtrl_glad{
	private:
		bool loaded = false;
		
		Upp::Vector<Object3D> allObjects;
		TrackBallCamera camera;
				
		OpenGLProgram DrawMeshNoLight;
		OpenGLProgram DrawMeshLight;
		OpenGLProgram DrawMeshLine;
		OpenGLProgram DrawMeshNormal;
		void InitShader();


		bool ShowAxis = false;
		bool pressed = false;
		Point StartPress;
		
		float sizeW = 800.0f;
		float sizeH = 600.0f;
		
		void ProcessMouse(float pitch, float yaw);
	public:
		SurfaceCtrl();
		~SurfaceCtrl();
		
		Function <void()> OnBegin;
		Function <void()> WhenPaint;
		Function <void()> OnEnd;
		
		TrackBallCamera& GetCamera()noexcept{return camera;}
		
		void CreateObject(Surface& surf,Upp::Color color)noexcept;
		
		void DrawAllObjects();
		
		void InitCamera()noexcept;
		void ZoomToFit()noexcept;
		void InitOpenGLFeatures()noexcept;
		
		virtual void GLPaint();
		virtual void GLResize(int w, int h);
		virtual bool Key(dword key,int count);
		virtual void MouseMove(Point p, dword);
		virtual void MouseWheel(Point p,int zdelta,dword keyflags);
		virtual void LeftDown(Point p, dword);
		virtual void LeftUp(Point p, dword);
		virtual void MouseLeave();
};
}
#endif
