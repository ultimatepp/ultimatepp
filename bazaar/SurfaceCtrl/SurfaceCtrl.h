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
#include "MagicCamera.h"
#include "Object3DProvider.h"

namespace Upp{
	
class SurfaceCtrl : public GLCtrl_glad{
	private:
		bool loaded = false;
		Object3DProvider objProvider;
		
		Upp::Array<Object3D> allObjects;
		Upp::Vector<Object3D*> SelectedObject;
		
		Object3D Axis;
		Object3D CameraFocus;
		Object3D SimpleCube; //Test object Should be deleted in release version
		Object3D SimpleCube2; //Test object Should be deleted in release version

		MagicCamera camera;
				
		OpenGLProgram DrawMeshNoLight;
		OpenGLProgram DrawMeshLight;
		OpenGLProgram DrawMeshLine;
		OpenGLProgram DrawMeshNormal;
		
		bool ShowAxis = true;
		bool ShowCameraFocus = false;
		
		float sizeW = 800.0f;
		float sizeH = 600.0f;
		
		bool TimerStarted = false;
		std::chrono::time_point<std::chrono::high_resolution_clock> start,end; //High resolution clock
		double DeltaTime=0.0f,LastTime=0.0f,lastFrame =0.0f,Timer=0.0f;
		int bufferFrame =0,frameCount = 0; //used to calculate FPS
		void ProcessTime();
		
		bool FastMode = false;

		void InitShader();
		
		
	public:
		SurfaceCtrl();
		~SurfaceCtrl();
		
		Function <void()> OnBegin;
		Function <void()> WhenPaint;
		Function <void()> OnEnd;
	
		MagicCamera& GetCamera()noexcept{return camera;}
		
		Object3D& CreateObject(Surface& surf,Upp::Color color)noexcept;
		void DeleteObject(unsigned int iterator);
		void DeleteSelectedObjects();
		void DeleteAllObjects();
		
		void DrawAllObjects();
		
		void InitCamera()noexcept;
		void ZoomToFit()noexcept;
		void InitOpenGLFeatures()noexcept;
		void ProcessSelectedObject(Point& p, dword keyflags)noexcept;
		
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
		
		//Fast Mode
		SurfaceCtrl& EnableFastMode(){FastMode = true;return *this;}
		SurfaceCtrl& DisableFastMode(){FastMode = false;return *this;}
		
		//time Management
		SurfaceCtrl& StartTimer(){TimerStarted = true; start= std::chrono::high_resolution_clock::now();return *this;}
		SurfaceCtrl& StopTimer(){TimerStarted = false;return *this;}
		
		double GetEllapsedTime(){
			if(TimerStarted){
				end = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> diff = end-start;
				Timer =diff.count();
				return Timer;
			}else{
				LOG("Timer has not been started, Start it by using SurfaceCtrl::StartTimer();");
				return 0;
			}
		}
		double GetDeltaTime(){
			if(TimerStarted){
				return DeltaTime;
			}else{
				LOG("Timer has not been started, Start it by using SurfaceCtrl::StartTimer();");
				return 0;
			}
		}
		

};
}
#endif
