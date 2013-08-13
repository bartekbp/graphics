#include "Variables.h"

ProgramData UniformColor;
ProgramData ObjectColor;
ProgramData UniformColorTint;
ShipProgramData ShipProgram;

Framework::Mesh *g_pConeMesh = NULL;
Framework::Mesh *g_pCylinderMesh = NULL;
Framework::Mesh *g_pPlaneMesh = NULL;
Framework::Mesh *g_pShip = NULL;

GLuint car_vao;
GLuint car_vbo;

std::vector<TreeData> g_forest;

float g_fYAngle = 0.0f;
float g_fXAngle = 0.0f;


bool g_bDrawLookatPoint = false;
glm::vec3 g_camTarget(0.0f, 0.4f, 0.0f);

//In spherical coordinates.
glm::vec3 g_sphereCamRelPos(67.5f, -46.0f, 150.0f);

float g_fzNear = 1.0f;
float g_fzFar = 1000.0f;


float g_fLightHeight = 3.0f;
float g_fLightRadius = 3.0f;

Car* car = NULL;

Framework::Timer g_LightTimer(Framework::Timer::TT_LOOP, 5.0f);