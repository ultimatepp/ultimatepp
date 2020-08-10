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
#include "SketchupCamera.h"
#include "Object3DProvider.h"

namespace Upp{
	
class SurfaceCtrl : public GLCtrl_glad{
	private:
		bool loaded = false;
		Object3DProvider objProvider;
		
		Upp::Vector<Object3D> allObjects;
		Upp::Vector<Object3D*> SelectedObject;
		
		Object3D Axis;
		Object3D CameraFocus;

		SketchupCamera camera;
				
		OpenGLProgram DrawMeshNoLight;
		OpenGLProgram DrawMeshLight;
		OpenGLProgram DrawMeshLine;
		OpenGLProgram DrawMeshNormal;
		
		bool ShowAxis = true;
		bool ShowCameraFocus = false;
		
		float sizeW = 800.0f;
		float sizeH = 600.0f;
		
		void InitShader();
	public:
		SurfaceCtrl();
		~SurfaceCtrl();
		
		Function <void()> OnBegin;
		Function <void()> WhenPaint;
		Function <void()> OnEnd;
		
	
		//FreeCamera& GetFreeCamera()noexcept{return euler;}
		SketchupCamera& GetSketchupCamera()noexcept{return camera;}
	//	TrackBallCamera& GetTrackBallCamera()noexcept{return camera;}
		
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
		virtual void MiddleDown(Point p, dword keyflags);
		virtual void MiddleUp(Point p, dword keyflags);

};
}
#endif
