#include "Variables.h"


using Framework::Timer;


void DrawTrunk(glutil::MatrixStack &modelMatrix, float fTrunkHeight)
{
	glutil::PushStack push(modelMatrix);

	modelMatrix.Scale(glm::vec3(1.0f, fTrunkHeight, 1.0f));
	modelMatrix.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

	glUseProgram(UniformColorTint.theProgram);
	glUniformMatrix4fv(UniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform4f(UniformColorTint.baseColorUnif, 0.55f, 0.35f, 0.056f, 1.0f);
	g_pCylinderMesh->Render();
	glUseProgram(0);
}

void DrawTreeTop(glutil::MatrixStack &modelMatrix, float fTrunkHeight, float fConeHeight)
{
	glutil::PushStack push(modelMatrix);

	modelMatrix.Translate(glm::vec3(0.0f, fTrunkHeight, 0.0f));
	modelMatrix.Scale(glm::vec3(3.0f, fConeHeight, 3.0f));

	glUseProgram(UniformColorTint.theProgram);
	glUniformMatrix4fv(UniformColorTint.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform4f(UniformColorTint.baseColorUnif, 0.0f, 0.4f, 0.0f, 1.0f);
	g_pConeMesh->Render();
	glUseProgram(0);
}

void DrawTree(glutil::MatrixStack &modelMatrix, float fTrunkHeight = 2.0f, float fConeHeight = 3.0f)
{
	DrawTrunk(modelMatrix, fTrunkHeight);
	DrawTreeTop(modelMatrix, fTrunkHeight, fConeHeight);
}


void DrawForest(glutil::MatrixStack &modelMatrix)
{
	for(unsigned int iTree = 0; iTree < g_forest.size(); iTree++)
	{
		const TreeData &currTree = g_forest[iTree];

		glutil::PushStack push(modelMatrix);
		modelMatrix.Translate(glm::vec3(currTree.fXPos, 0.0f, currTree.fZPos));
		DrawTree(modelMatrix, currTree.fTrunkHeight, currTree.fConeHeight);
	}
}

void DrawLight(glutil::MatrixStack &modelMatrix, float moveRight, float yScale, glm::vec4 color)
{
	glutil::PushStack push(modelMatrix);

	modelMatrix.Translate(car->x(), 5.0f, car->z());
	modelMatrix.RotateY(glm::degrees(car->degree()));
	modelMatrix.Translate(moveRight, 0, 0);
	modelMatrix.RotateZ(90);
	modelMatrix.Scale(1.0f, yScale, 1.0f);

	glUseProgram(UniformColor.theProgram);
	glUniformMatrix4fv(UniformColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform4f(UniformColor.baseColorUnif, color.r, color.g, color.b, color.a);	
	g_pCylinderMesh->Render();

	glUseProgram(0);
}

void DrawCarLights(glutil::MatrixStack &modelMatrix)
{
	DrawLight(modelMatrix, 0.6f, 1.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	DrawLight(modelMatrix, 0.0f, 0.25f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	DrawLight(modelMatrix, -0.6f, 1.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
}


void DrawCar(glutil::MatrixStack &modelMatrix)
{
	glutil::PushStack push(modelMatrix);

	modelMatrix.Translate(car->x(), 0, car->z());
	modelMatrix.Scale(car->scale());
	modelMatrix.RotateY(glm::degrees(car->degree()));

	glUseProgram(UniformColor.theProgram);
	glUniformMatrix4fv(UniformColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform4f(UniformColor.baseColorUnif, 0.2f, 0.2f, 0.7f, 1.0f);
	car->Render();

	glUseProgram(0);
}

void SendReflectorPosition(glutil::MatrixStack &modelMatrix, GLuint reflector, float relativePositionX, float relativePositionY, float relativePositionZ)
{
	glutil::PushStack push(modelMatrix);
	modelMatrix.SetIdentity();

	glm::vec4 basePosition(relativePositionX, relativePositionY, relativePositionZ, 1.0f);
	modelMatrix.Translate(car->x(), 0.0f, car->z());
	modelMatrix.Scale(car->scale());
	modelMatrix.RotateY(glm::degrees(car->degree()));

	glUniform4fv(reflector, 1, glm::value_ptr(modelMatrix.Top() * basePosition));
}

void DrawShip(glutil::MatrixStack &modelMatrix)
{
	glutil::PushStack push(modelMatrix);

	glm::mat4 worldToCamera = modelMatrix.Top();

	modelMatrix.SetIdentity();
	modelMatrix.Translate(glm::vec3(0.0f, 2.0f, 50.0f));
	modelMatrix.Scale(3);
	modelMatrix.Rotate(glm::vec3(0, 1.0f, 0), -34);
	modelMatrix.Rotate(glm::vec3(0.5, 0.0, 0.5), 10);

	
	glUseProgram(ShipProgram.theProgram);

	glUniform1f(ShipProgram.shininess, 2.3f);
	glUniform3f(ShipProgram.ks, 0.5f, 0.5f, 0.5f);
	glUniform3f(ShipProgram.ka, 0.2f, 0.2f, 0.2f);
	glUniform3f(ShipProgram.kd, 0.3f, 0.3f, 0.3f);

	glUniform1f(ShipProgram.spotLight.cutoffUnif, 73.0);
	glUniform1f(ShipProgram.spotLight.exponentUnif, 3.0);

	glUniform3fv(ShipProgram.spotLight.directionUnif, 1, glm::value_ptr(car->reflectorDirection()));
	glUniform3fv(ShipProgram.spotLight.intensityUnif, 1, glm::value_ptr(glm::vec3(0.8, 0.8, 0.8)));

	SendReflectorPosition(modelMatrix, ShipProgram.spotLight.positionLeftReflectorUnif, -0.2f, 0.2f, -0.5f);
	SendReflectorPosition(modelMatrix, ShipProgram.spotLight.positionRightReflectorUnif, 0.2f, 0.2f, -0.5f);

	glUniformMatrix4fv(ShipProgram.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniformMatrix4fv(ShipProgram.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(worldToCamera));
	g_pShip->Render();

	glUseProgram(0);
}

void DrawRoad(glutil::MatrixStack &modelMatrix)
{
	glutil::PushStack push(modelMatrix);
	glDisable(GL_DEPTH_TEST);

	modelMatrix.Scale(glm::vec3(30.0f, 1.0f, 100.0f));
	modelMatrix.Translate(0.0f, 0.0f, -0.5f);

	glUseProgram(UniformColor.theProgram);
	glUniformMatrix4fv(UniformColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform4f(UniformColor.baseColorUnif, 0.22f, 0.21f, 0.15f, 1.0f);
	g_pPlaneMesh->Render();

	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
}

void DrawUniformGround(glutil::MatrixStack &modelMatrix)
{
	glutil::PushStack push(modelMatrix);

	modelMatrix.Scale(glm::vec3(100.0f, 1.0f, 200.0f));

	glUseProgram(UniformColor.theProgram);
	glUniformMatrix4fv(UniformColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform4f(UniformColor.baseColorUnif, 0.22f, 0.12f, 0.07f, 1.0f);
	g_pPlaneMesh->Render();

	glUseProgram(0);
}

void DrawGround(glutil::MatrixStack &modelMatrix)
{
	DrawUniformGround(modelMatrix);
	DrawRoad(modelMatrix);
}

void DrawLookAtPoint(glutil::MatrixStack &modelMatrix)
{
	glDisable(GL_DEPTH_TEST);
	glutil::PushStack push(modelMatrix);

	modelMatrix.SetIdentity();
	modelMatrix.Translate(glm::vec3(0.0f, 0.0f, -g_viewPole.GetView().radius));
		
	glUseProgram(ObjectColor.theProgram);
	glUniformMatrix4fv(ObjectColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	g_pCylinderMesh->Render();

	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
}

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(g_pConeMesh && g_pCylinderMesh && g_pPlaneMesh)
	{
		glutil::MatrixStack modelMatrix;
		modelMatrix.SetMatrix(g_viewPole.CalcMatrix());

		DrawGround(modelMatrix);
		
		DrawForest(modelMatrix);

		DrawShip(modelMatrix);

		DrawCar(modelMatrix);
		DrawCarLights(modelMatrix);

		if(g_bDrawLookatPoint)
		{
			DrawLookAtPoint(modelMatrix);
		}

	}

	glutPostRedisplay();
	glutSwapBuffers();
}

