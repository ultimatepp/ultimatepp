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
		
		Upp::Vector<Object3D> allObjects;
		Upp::Vector<int> allSelected;
		
		Object3D Axis;
		Object3D CameraFocus;
		Skybox skybox;

		MagicCamera camera;
				
		OpenGLProgram DrawMeshNoLight;
		OpenGLProgram DrawMeshLight;
		OpenGLProgram DrawMeshLine;
		OpenGLProgram DrawMeshNormal;
		
		bool showAxis = true;
		bool ShowCameraFocus = false;
		
		float sizeW = 800.0f;
		float sizeH = 600.0f;
		
		bool TimerStarted = false;
		std::chrono::time_point<std::chrono::high_resolution_clock> start,end; //High resolution clock
		double DeltaTime=0.0f,LastTime=0.0f,lastFrame =0.0f,Timer=0.0f;
		int bufferFrame =0,frameCount = 0; //used to calculate FPS
		void ProcessTime()noexcept;
		
		bool fastMode = false;

		void InitShader(); //Load default shader
	public:
		SurfaceCtrl();
		~SurfaceCtrl();
		
		Function <void()> OnBegin;
		Function <void()> WhenPaint;
		Function <void()> OnEnd;
		
		//Starting function
		void InitCamera()noexcept;
		void InitOpenGLFeatures()noexcept;

		//Action on all objects vector
		const Upp::Vector<Object3D>& GetAllObjects()const noexcept{return allObjects;}
		Object3D& CreateObject()noexcept;
		int FindObject(int ID)const noexcept;
		Object3D& GetObject(int ID)noexcept;
		void DeleteObject(int ID)noexcept; //Delete the object (update selected)
		void DeleteAllObjects()noexcept; //Delete all object
		void SetDefaultShader(Object3D& obj); //Set default to the object in arg
		void DrawAllObjects()noexcept; //Draw all object

		//Change selected object vector
		const Upp::Vector<int>& GetSelectedObject()const noexcept; //return const vector representing all selected Object
		void AddSelectedObject(int ID)noexcept;
		void UpdateSelectedObjectViaMouse(Point& p, dword keyflags)noexcept; //Process work on selected object depending on keyflags and point
		glm::vec3 GetCenterPoint()const noexcept; //Return center point between all selected item
		void RemoveSelectedObject(int ID)noexcept;
		void ClearSelectedObject()noexcept;
		
		//Change Object selected
		void MoveAllSelectedObjects(glm::vec3 move)noexcept; //Move all selected object
		void RotateAllSelectedObjects(glm::quat rotation)noexcept; //Rotate all selected object
		void DeleteAllSelectedObjects()noexcept; //Delete all selected object
		
		//Camera getter
		const MagicCamera& GetCamera()const noexcept{return camera;}
		MagicCamera& GetCamera()noexcept{return camera;}
		
		//Fast Mode
		SurfaceCtrl& EnableFastMode()noexcept{fastMode = true;return *this;}
		SurfaceCtrl& DisableFastMode()noexcept{fastMode = false;return *this;}
		SurfaceCtrl& FastMode(bool b = true)noexcept{fastMode = b; return *this;}
		bool IsfastModeEnable()const noexcept{return fastMode;}
		
		//time option
		SurfaceCtrl& StartTimer()noexcept{TimerStarted = true; start= std::chrono::high_resolution_clock::now();return *this;}
		SurfaceCtrl& StopTimer()noexcept{TimerStarted = false;return *this;}
		double GetEllapsedTime()noexcept;
		double GetDeltaTime()noexcept;
		
		//Axis option
		SurfaceCtrl& EnableAxis()noexcept{showAxis = true; return *this;}
		SurfaceCtrl& DisableAxis()noexcept{showAxis = false; return *this;}
		SurfaceCtrl& ShowAxis(bool b = true)noexcept{showAxis = b; return *this;}
		bool IsAxisEnable()const noexcept{return showAxis;}
		
		//Application event
		virtual void GLPaint(); //paint function
		virtual void GLResize(int w, int h); //Action on resize
		
		//Input event
		virtual void MouseMove(Point p, dword); //Action on mouse move
		virtual void MouseWheel(Point p,int zdelta,dword keyflags); //action on Mouse wheel
		virtual void LeftDown(Point p, dword); //Action on Left Down mouse
		virtual void LeftUp(Point p, dword); //Action on Up down mouse
		virtual void MiddleDown(Point p, dword keyflags); //Action Middle down (wheel down) mouse
		virtual void MiddleUp(Point p, dword keyflags);//Action Middle up (wheel up) mouse
		virtual void MouseLeave(); //action when mouse leave
		virtual bool Key(dword key,int count); //Action when key press
};
}
#endif
