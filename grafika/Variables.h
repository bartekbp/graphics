#pragma once

#include <glload/gl_3_3.h>
#include <glutil/glutil.h>
#include <GL/freeglut.h>
#include "../framework/framework.h"
#include "../framework/Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Car.h"
#include "../framework/Timer.h"

#include <vector>

#define COUNT(a) sizeof(a) / sizeof(a[0])

struct SpotLightInfo {
    GLuint positionLeftReflectorUnif;		// Position in world coords
	GLuint positionRightReflectorUnif;		// Position in world coords
    GLuint intensityUnif;
    GLuint directionUnif;		// Direction of the spotlight in model coords.
    GLuint exponentUnif;		// Angular attenuation exponent
    GLuint cutoffUnif;			// Cutoff angle (between 0 and 90)
};

struct ReflectorsProgramData
{
	GLuint theProgram;
	
	SpotLightInfo spotLight;

	GLuint kd;					// Diffuse reflectivity
	GLuint ka;					// Ambient reflectivity
	GLuint ks;					// Specular reflectivity
	GLuint shininess;			// Specular shininess factor

	GLuint modelToWorldMatrixUnif;
	GLuint worldToCameraMatrixUnif;
	GLuint cameraToClipMatrixUnif;
};

struct ReflectorsAndUniformColorProgramData
{
	GLuint theProgram;

	SpotLightInfo spotLight;

	GLuint ka;					// Ambient reflectivity

	GLuint modelToWorldMatrixUnif;
	GLuint worldToCameraMatrixUnif;
	GLuint cameraToClipMatrixUnif;

	GLuint baseColorUnif;

	GLuint worldSpaceLightPosUnif;
	GLuint lightIntensityUnif;
};

struct ReflectorsAndLightProgramData
{
	struct ReflectorsProgramData reflectorsProgramData;

	GLuint worldSpaceLightPosUnif;
	GLuint lightIntensityUnif;
};

struct ProgramData
{
	GLuint theProgram;
	GLuint modelToWorldMatrixUnif;
	GLuint cameraToClipMatrixUnif;
	GLuint baseColorUnif;
};

struct TreeData
{
	float fXPos;
	float fZPos;
	float fTrunkHeight;
	float fConeHeight;
};


extern GLuint g_projectionUniformBuffer;

extern ProgramData ColorProvided;
extern ProgramData UniformColor;
extern ReflectorsAndLightProgramData ReflectorsAndLightProgram;
extern ReflectorsAndUniformColorProgramData UniformColorAndLightProgram;



extern Framework::Mesh *g_pConeMesh;
extern Framework::Mesh *g_pCylinderMesh;
extern Framework::Mesh *g_pPlaneMesh;
extern Framework::Mesh *g_pShip;
extern Framework::Mesh *g_pTetrahedronMesh;
extern Framework::Mesh *g_pSphereMesh;

extern Car *car;

extern std::vector<TreeData> g_forest;


extern bool g_bDrawLookatPoint;

extern float g_fzNear;
extern float g_fzFar;

extern float g_fLightHeight;
extern float g_fLightRadius;

extern float g_SphereDiffHeight;
extern float g_fSphereBaseHeight;
extern float g_fSphereRadius;

extern Framework::Timer g_LightTimer;
extern Framework::Timer g_TetrahedronTimer;
extern Framework::Timer g_SphereTimer;

extern glutil::ViewPole g_viewPole;