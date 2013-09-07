#include "Variables.h"

ProgramData ColorProvided;
ProgramData UniformColor;
ReflectorsAndLightProgramData ReflectorsAndLightProgram;
ReflectorsAndUniformColorProgramData UniformColorAndLightProgram;

Framework::Mesh *g_pConeMesh = NULL;
Framework::Mesh *g_pCylinderMesh = NULL;
Framework::Mesh *g_pPlaneMesh = NULL;
Framework::Mesh *g_pShip = NULL;
Framework::Mesh *g_pTetrahedronMesh = NULL;
Framework::Mesh *g_pSphereMesh = NULL;

GLuint car_vao;
GLuint car_vbo;

std::vector<TreeData> g_forest;

bool g_bDrawLookatPoint = false;

float g_fzNear = 1.0f;
float g_fzFar = 1000.0f;


float g_fLightHeight = 0.0f;
float g_fLightRadius = 30.0f;

float g_SphereDiffHeight = 20.0f;
float g_fSphereBaseHeight = 30.f;
float g_fSphereRadius = 70.0f;

Car* car = NULL;

Framework::Timer g_LightTimer(Framework::Timer::TT_LOOP, 5.0f);
Framework::Timer g_TetrahedronTimer(Framework::Timer::TT_LOOP, 10.0f);
Framework::Timer g_SphereTimer(Framework::Timer::TT_LOOP, 4.0f);

glutil::ViewData g_initialViewData =
{
	glm::vec3(0.0f, 0.0f, -80.0f),
	glm::fquat(0.92387953f, 0.3826834f, 0.0f, 0.0f),
	100.0f,
	0.0f
};

glutil::ViewScale g_viewScale =
{
	0.0f, 200.0f,
	10.0f, 1.0f,
	30.0f, 5.0f,
	90.0f/250.0f
};

glutil::ObjectData g_initialObjectData =
{
	glm::vec3(0.0f, 20.0f, 0.0f),
	glm::fquat(0.0f, 1.0f, 0.0f, 0.0f),
};

glutil::ViewPole g_viewPole = glutil::ViewPole(g_initialViewData, g_viewScale, glutil::MB_LEFT_BTN);