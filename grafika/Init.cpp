#include <string>
#include <vector>
#include <stack>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include "Variables.h"
#include "Utils.h"

void MouseMotion(int x, int y);
void MouseButton(int button, int state, int x, int y);
void MouseWheel(int wheel, int direction, int x, int y);

ProgramData LoadProgram(const std::string &strVertexShader, const std::string &strFragmentShader)
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(Framework::LoadShader(GL_VERTEX_SHADER, strVertexShader));
	shaderList.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER, strFragmentShader));

	ProgramData data;
	data.theProgram = Framework::CreateProgram(shaderList);
	data.modelToWorldMatrixUnif = glGetUniformLocation(data.theProgram, "modelToWorldMatrix");
	data.cameraToClipMatrixUnif = glGetUniformLocation(data.theProgram, "cameraToClipMatrix");
	data.baseColorUnif = glGetUniformLocation(data.theProgram, "baseColor");

	return data;
}


ReflectorsProgramData LoadReflectorsProgram(std::string vertexShader, std::string fragmentShader)
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(Framework::LoadShader(GL_VERTEX_SHADER, vertexShader));
	shaderList.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER, fragmentShader));

	ReflectorsProgramData data;
	data.theProgram = Framework::CreateProgram(shaderList);
	
	data.spotLight.positionLeftReflectorUnif = glGetUniformLocation(data.theProgram, "Spot.positionLeftReflector");
	data.spotLight.positionRightReflectorUnif = glGetUniformLocation(data.theProgram, "Spot.positionRightReflector");
	data.spotLight.intensityUnif = glGetUniformLocation(data.theProgram, "Spot.intensity");
	data.spotLight.directionUnif = glGetUniformLocation(data.theProgram, "Spot.direction");
	data.spotLight.exponentUnif = glGetUniformLocation(data.theProgram, "Spot.exponent");
	data.spotLight.cutoffUnif = glGetUniformLocation(data.theProgram, "Spot.cutoff");
	
	data.kd = glGetUniformLocation(data.theProgram, "Kd");
	data.ka = glGetUniformLocation(data.theProgram, "Ka");
	data.ks = glGetUniformLocation(data.theProgram, "Ks");
	data.shininess = glGetUniformLocation(data.theProgram, "Shininess");


	data.cameraToClipMatrixUnif = glGetUniformLocation(data.theProgram, "cameraToClipMatrix");
	data.modelToWorldMatrixUnif = glGetUniformLocation(data.theProgram, "modelToWorldMatrix");
	data.worldToCameraMatrixUnif = glGetUniformLocation(data.theProgram, "worldToCameraMatrix");

	return data;
}

ReflectorsAndLightProgramData LoadReflectorsAndLightsProgram(std::string vertexShader, std::string fragmentShader)
{
	ReflectorsProgramData reflectorsData = LoadReflectorsProgram(vertexShader, fragmentShader);

	ReflectorsAndLightProgramData data;
	data.reflectorsProgramData = reflectorsData;
	data.worldSpaceLightPosUnif = glGetUniformLocation(data.reflectorsProgramData.theProgram, "worldSpaceLightPos");
	data.lightIntensityUnif = glGetUniformLocation(data.reflectorsProgramData.theProgram, "lightIntensity");

	return data;
}

ReflectorsAndUniformColorProgramData LoadReflectorsAndUniformColorProgram(std::string vertexShader, std::string fragmentShader)
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(Framework::LoadShader(GL_VERTEX_SHADER, vertexShader));
	shaderList.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER, fragmentShader));

	ReflectorsAndUniformColorProgramData data;
	data.theProgram = Framework::CreateProgram(shaderList);
	
	data.spotLight.positionLeftReflectorUnif = glGetUniformLocation(data.theProgram, "Spot.positionLeftReflector");
	data.spotLight.positionRightReflectorUnif = glGetUniformLocation(data.theProgram, "Spot.positionRightReflector");
	data.spotLight.intensityUnif = glGetUniformLocation(data.theProgram, "Spot.intensity");
	data.spotLight.directionUnif = glGetUniformLocation(data.theProgram, "Spot.direction");
	data.spotLight.exponentUnif = glGetUniformLocation(data.theProgram, "Spot.exponent");
	data.spotLight.cutoffUnif = glGetUniformLocation(data.theProgram, "Spot.cutoff");
	
	data.ka = glGetUniformLocation(data.theProgram, "Ka");
	data.baseColorUnif = glGetUniformLocation(data.theProgram, "baseColor");

	data.worldSpaceLightPosUnif = glGetUniformLocation(data.theProgram, "worldSpaceLightPos");
	data.lightIntensityUnif = glGetUniformLocation(data.theProgram, "lightIntensity");

	data.cameraToClipMatrixUnif = glGetUniformLocation(data.theProgram, "cameraToClipMatrix");
	data.modelToWorldMatrixUnif = glGetUniformLocation(data.theProgram, "modelToWorldMatrix");
	data.worldToCameraMatrixUnif = glGetUniformLocation(data.theProgram, "worldToCameraMatrix");

	return data;
}

void InitializeProgram()
{
	UniformColor = LoadProgram("PosOnlyWorldTransform.vert", "ColorUniform.frag");
	ColorProvided = LoadProgram("PosColorWorldTransform.vert", "ColorMultUniform.frag");
	UniformColorAndLightProgram = LoadReflectorsAndUniformColorProgram("ColorUniformAndLight.vert", "ColorUniformAndLight.frag");
	ReflectorsAndLightProgram = LoadReflectorsAndLightsProgram("ReflectorsAndLight.vert", "ReflectorsAndLight.frag");
}

TreeData InitializeTree(float sx, float ex, float stepx, float sz, float ez, float stepz, float st, float et, float sc, float ec)
{
	float x = std::min(ex, RandWithStep(sx, ex, stepx));
	float z = std::min(ez, RandWithStep(sz, ez, stepz));
	float trunk = RandWithStep(st, et, 0.15f);
	float cone = RandWithStep(sc, ec, 0.24f);
	TreeData tree = {x, z, trunk, cone};
	return tree;
}

void InitializeTrees(float startx, float endx, float startz, float endz, int count)
{
	for(int i = 0; i < count; i++)
	{
		TreeData tree = InitializeTree(startx, endx, 0.4f, startz, endz, 0.4f, 1.2f, 5, 4.8f, 11.9f);
		g_forest.push_back(tree);
	}
}

void InitializeForest()
{
	srand((unsigned int) time(0));
	InitializeTrees(-49, -14, -99, 0, 130);
	InitializeTrees(16, 49, -99, 0, 130);
}

bool emptySpace(GLfloat x, GLfloat z)
{
	if(x < -45 || x > 45)
	{
		return false;
	}

	if(z < -95 || z > 95)
	{
		return false;
	}

	if(z < 5 && (x < -10 || x > 10))
	{
		return false;
	}

	if(z < 70 && z > 33 && x < 23 && x > -23)
	{
		return false;
	}

	if(z > 60 && x > 30)
	{
		return false;
	}

	return true;
}

void InitializeShaders()
{
	try
	{
		g_pConeMesh = new Framework::Mesh("UnitConeTint.xml");
		g_pShip = new Framework::Mesh("Ship.xml");
		g_pCylinderMesh = new Framework::Mesh("UnitCylinderTint.xml");
		g_pPlaneMesh = new Framework::Mesh("UnitPlane.xml");
		g_pTetrahedronMesh = new Framework::Mesh("UnitTetrahedron.xml");
		g_pSphereMesh = new Framework::Mesh("UnitSphere.xml");
	}
	catch(std::exception &except)
	{
		printf("%s\n", except.what());
		throw;
	}
}

void init()
{
	InitializeProgram();

	InitializeForest();

	Car::Initialize();
	car->emptySpaceHandler(emptySpace);
	
	InitializeShaders();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
	glEnable(GL_DEPTH_CLAMP);

	glutMouseFunc(MouseButton);
 	glutMotionFunc(MouseMotion);
	glutMouseWheelFunc(MouseWheel);
}