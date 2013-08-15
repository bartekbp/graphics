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
    GLuint positionLeftUnif;		// Position in world coords
	GLuint positionRightUnif;		// Position in world coords
    GLuint intensityUnif;
    GLuint directionUnif;		// Direction of the spotlight in model coords.
    GLuint exponentUnif;		// Angular attenuation exponent
    GLuint cutoffUnif;			// Cutoff angle (between 0 and 90)
};

struct ShipProgramData
{
	GLuint theProgram;
	
	SpotLightInfo spotLight;

	GLuint kd;					// Diffuse reflectivity
	GLuint ka;					// Ambient reflectivity
	GLuint ks;					// Specular reflectivity
	GLuint shininess;			// Specular shininess factor

	GLuint worldToCameraMatrixUnif;
	GLuint modelToWorldMatrixUnif;
	GLuint cameraToClipMatrixUnif;
};

struct ProgramData
{
	GLuint theProgram;
	GLuint modelToWorldMatrixUnif;
	GLuint worldToCameraMatrixUnif;
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


extern ProgramData UniformColor;
extern ProgramData ObjectColor;
extern ProgramData UniformColorTint;
extern ShipProgramData ShipProgram;


extern Framework::Mesh *g_pConeMesh;
extern Framework::Mesh *g_pCylinderMesh;
extern Framework::Mesh *g_pPlaneMesh;
extern Framework::Mesh *g_pShip;

extern Car *car;

extern std::vector<TreeData> g_forest;

extern float g_fYAngle;
extern float g_fXAngle;


extern bool g_bDrawLookatPoint;
extern glm::vec3 g_camTarget;

//In spherical coordinates.
extern glm::vec3 g_sphereCamRelPos;

extern float g_fzNear;
extern float g_fzFar;

extern float g_fLightHeight;
extern float g_fLightRadius;

extern Framework::Timer g_LightTimer;