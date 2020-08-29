#ifndef _SurfaceCtrl_Definition_h_
#define _SurfaceCtrl_Definition_h_
#include <CtrlLib/CtrlLib.h>
#include <Surface/Surface.h>

#ifdef flagGLCTRL
#	include <GLCtrl/GLCtrl.h>
#	define GL GLCtrl
#else
#	include <GLCtrl_glad/GLCtrl_glad.h>
#	define GL GLCtrl_glad
#endif


#include <plugin/assimp/assimp.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <plugin/glm/glm.hpp>
#include <plugin/glm/gtc/matrix_transform.hpp>
#include <plugin/glm/gtc/type_ptr.hpp>
#include <plugin/glm/gtx/quaternion.hpp>
#include <plugin/glm/ext/quaternion_trigonometric.hpp>
#include <plugin/glm/gtc/constants.hpp>
#include <plugin/glm/gtx/norm.hpp>
#include <plugin/glm/gtx/compatibility.hpp>
#include <plugin/glm/gtx/string_cast.hpp>

#define STRINGIFY(...) #__VA_ARGS__
#define SHADER(version, shader) "#version " #version "\n" STRINGIFY(shader)

namespace Upp{
	enum Camera_Movement {CM_FORWARD,CM_BACKWARD,CM_LEFT,CM_RIGHT};// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
	enum CameraType{CT_PERSPECTIVE = 0 ,CT_ORTHOGRAPHIC = 1};//,CT_FRUSTUM = 2}; //Type of camera rendering
	enum DrawType { DT_TRIANGLE, DT_QUAD };
	enum TexturesMaterial { TM_WATER, TM_STONE, TM_BRICK, TM_METAL, TM_WOOD};

	class SurfaceCtrl;
	class Skybox;
	class Object3D;
	class Object3DProvider;
	
	class Mesh;
	class BoundingBox;
	class OpenGLShader;
	class OpenGLProgram;
	
	class Transform;
	
	class UOGL_Camera;
	class MagicCamera;
	
	struct Texture;
	struct Material;
}
	
#endif
