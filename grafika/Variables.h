#pragma once

#include <glload/gl_3_3.h>
#include <glutil/glutil.h>
#include <GL/freeglut.h>
#include "../framework/framework.h"
#include "../framework/Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Car.h"

#include <vector>

#define COUNT(a) sizeof(a) / sizeof(a[0])

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


extern ProgramData UniformColor;
extern ProgramData ObjectColor;
extern ProgramData UniformColorTint;

extern Framework::Mesh *g_pConeMesh;
extern Framework::Mesh *g_pCylinderMesh;
extern Framework::Mesh *g_pCubeColorMesh;
extern Framework::Mesh *g_pPlaneMesh;

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