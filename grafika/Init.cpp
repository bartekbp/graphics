#include <string>
#include <vector>
#include <stack>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include "Variables.h"
#include "Utils.h"

ProgramData LoadProgram(const std::string &strVertexShader, const std::string &strFragmentShader)
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(Framework::LoadShader(GL_VERTEX_SHADER, strVertexShader));
	shaderList.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER, strFragmentShader));

	ProgramData data;
	data.theProgram = Framework::CreateProgram(shaderList);
	data.modelToWorldMatrixUnif = glGetUniformLocation(data.theProgram, "modelToWorldMatrix");
	data.worldToCameraMatrixUnif = glGetUniformLocation(data.theProgram, "worldToCameraMatrix");
	data.cameraToClipMatrixUnif = glGetUniformLocation(data.theProgram, "cameraToClipMatrix");
	data.baseColorUnif = glGetUniformLocation(data.theProgram, "baseColor");

	return data;
}

void InitializeProgram()
{
	UniformColor = LoadProgram("PosOnlyWorldTransform.vert", "ColorUniform.frag");
	ObjectColor = LoadProgram("PosColorWorldTransform.vert", "ColorPassthrough.frag");
	UniformColorTint = LoadProgram("PosColorWorldTransform.vert", "ColorMultUniform.frag");
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
	InitializeTrees(-50, -15, -100, 0, 130);
	InitializeTrees(15, 50, -100, 0, 130);
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

	if(z < 78 && z > 27 && x < 23 && x > -23)
	{
		return false;
	}

	return true;
}

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init()
{
	InitializeProgram();

	InitializeForest();

	Car::Initialize();
	car->emptySpaceHandler(emptySpace);


	try
	{
		g_pConeMesh = new Framework::Mesh("UnitConeTint.xml");
		g_pCylinderMesh = new Framework::Mesh("UnitCylinderTint.xml");
		g_pCubeColorMesh = new Framework::Mesh("UnitCubeColor.xml");
		g_pPlaneMesh = new Framework::Mesh("UnitPlane.xml");
	}
	catch(std::exception &except)
	{
		printf("%s\n", except.what());
		throw;
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
	glEnable(GL_DEPTH_CLAMP);
}