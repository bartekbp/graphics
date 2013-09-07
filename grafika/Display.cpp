#include "Variables.h"
#include <iostream>

using Framework::Timer;

glm::vec4 CalcLightPosition()
{
	g_LightTimer.Update();
	float fCurrTimeThroughLoop = g_LightTimer.GetAlpha();

	glm::vec4 ret(0.0f, g_fLightHeight, 0.0f, 1.0f);

	ret.x = cosf(fCurrTimeThroughLoop * (3.14159f * 2.0f)) * g_fLightRadius;
	ret.z = sinf(fCurrTimeThroughLoop * (3.14159f * 2.0f)) * g_fLightRadius;

	return ret + glm::vec4(0.0f, 20.0f, 80.0f, 1.0f);
}

void DrawTrunk(glutil::MatrixStack &modelMatrix, float fTrunkHeight)
{
	glutil::PushStack push(modelMatrix);

	modelMatrix.Scale(glm::vec3(1.0f, fTrunkHeight, 1.0f));
	modelMatrix.Translate(glm::vec3(0.0f, 0.5f, 0.0f));

	glUseProgram(ColorProvided.theProgram);
	glUniformMatrix4fv(ColorProvided.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform4f(ColorProvided.baseColorUnif, 0.55f, 0.35f, 0.056f, 1.0f);
	g_pCylinderMesh->Render();
	glUseProgram(0);
}

void DrawTreeTop(glutil::MatrixStack &modelMatrix, float fTrunkHeight, float fConeHeight)
{
	glutil::PushStack push(modelMatrix);

	modelMatrix.Translate(glm::vec3(0.0f, fTrunkHeight, 0.0f));
	modelMatrix.Scale(glm::vec3(3.0f, fConeHeight, 3.0f));

	glUseProgram(ColorProvided.theProgram);
	glUniformMatrix4fv(ColorProvided.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform4f(ColorProvided.baseColorUnif, 0.0f, 0.4f, 0.0f, 1.0f);
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

void DrawCarLight(glutil::MatrixStack &modelMatrix, float moveRight, float yScale, glm::vec4 color)
{
	glutil::PushStack push(modelMatrix);

	modelMatrix.Translate(car->x(), 5.0f, car->z());
	modelMatrix.RotateY(glm::degrees(car->degree()));
	modelMatrix.Translate(moveRight, 0, 0);
	modelMatrix.RotateZ(90);
	modelMatrix.Scale(1.0f, yScale, 1.0f);

	glUseProgram(ColorProvided.theProgram);
	glUniformMatrix4fv(ColorProvided.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform4f(ColorProvided.baseColorUnif, color.r, color.g, color.b, color.a);	
	g_pCylinderMesh->Render();

	glUseProgram(0);
}

void DrawCarLights(glutil::MatrixStack &modelMatrix)
{
	DrawCarLight(modelMatrix, 0.6f, 1.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	DrawCarLight(modelMatrix, 0.0f, 0.2f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	DrawCarLight(modelMatrix, -0.6f, 1.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
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

void SendReflectorPosition(GLuint reflector, float relativePositionX, float relativePositionY, float relativePositionZ)
{
	glutil::MatrixStack modelMatrix;

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

	glUseProgram(ReflectorsAndLightProgram.reflectorsProgramData.theProgram);

	glUniform1f(ReflectorsAndLightProgram.reflectorsProgramData.shininess, 2.3f);
	glUniform3f(ReflectorsAndLightProgram.reflectorsProgramData.ks, 0.5f, 0.5f, 0.5f);
	glUniform3f(ReflectorsAndLightProgram.reflectorsProgramData.ka, 0.3f, 0.3f, 0.3f);
	glUniform3f(ReflectorsAndLightProgram.reflectorsProgramData.kd, 0.3f, 0.3f, 0.3f);

	glUniform1f(ReflectorsAndLightProgram.reflectorsProgramData.spotLight.cutoffUnif, 73.0);
	glUniform1f(ReflectorsAndLightProgram.reflectorsProgramData.spotLight.exponentUnif, 3.0);

	glUniform3fv(ReflectorsAndLightProgram.reflectorsProgramData.spotLight.directionUnif, 1, glm::value_ptr(car->reflectorDirection()));
	glUniform3fv(ReflectorsAndLightProgram.reflectorsProgramData.spotLight.intensityUnif, 1, glm::value_ptr(glm::vec3(0.8, 0.8, 0.8)));

	SendReflectorPosition(ReflectorsAndLightProgram.reflectorsProgramData.spotLight.positionLeftReflectorUnif, -0.2f, 0.2f, 0.5f);
	SendReflectorPosition(ReflectorsAndLightProgram.reflectorsProgramData.spotLight.positionRightReflectorUnif, 0.2f, 0.2f, 0.5f);

	glUniformMatrix4fv(ReflectorsAndLightProgram.reflectorsProgramData.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniformMatrix4fv(ReflectorsAndLightProgram.reflectorsProgramData.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(worldToCamera));

	glm::vec3 worldLightPos(CalcLightPosition());
	
	glUniform3fv(ReflectorsAndLightProgram.worldSpaceLightPosUnif, 1, glm::value_ptr(worldLightPos));

	g_pShip->Render();

	glUseProgram(0);
}



void DrawRoad(glutil::MatrixStack &modelMatrix)
{
	glutil::PushStack push(modelMatrix);
	
	glDisable(GL_DEPTH_TEST);
	glm::mat4 worldToCamera = modelMatrix.Top();

	modelMatrix.SetIdentity();
	
	modelMatrix.Scale(glm::vec3(30.0f, 1.0f, 100.0f));
	modelMatrix.Translate(0.0f, 0.0f, -0.5f);

	glUseProgram(UniformColorAndLightProgram.theProgram);
	glUniformMatrix4fv(UniformColorAndLightProgram.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));

	glUniform3f(UniformColorAndLightProgram.ka, 0.3f, 0.3f, 0.3f);

	glUniform1f(UniformColorAndLightProgram.spotLight.cutoffUnif, 73.0);
	glUniform1f(UniformColorAndLightProgram.spotLight.exponentUnif, 3.0);

	glUniform3fv(UniformColorAndLightProgram.spotLight.directionUnif, 1, glm::value_ptr(car->reflectorDirection()));
	glUniform3fv(UniformColorAndLightProgram.spotLight.intensityUnif, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));

	SendReflectorPosition(UniformColorAndLightProgram.spotLight.positionLeftReflectorUnif, -0.2f, 0.2f, 0.5f);
	SendReflectorPosition(UniformColorAndLightProgram.spotLight.positionRightReflectorUnif, 0.2f, 0.2f, 0.5f);

	glUniformMatrix4fv(UniformColorAndLightProgram.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniformMatrix4fv(UniformColorAndLightProgram.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(worldToCamera));

	glUniformMatrix4fv(UniformColorAndLightProgram.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform4f(UniformColorAndLightProgram.baseColorUnif, 0.72f, 0.71f, 0.35f, 1.0f);

	glUniform4fv(UniformColorAndLightProgram.lightIntensityUnif, 1, glm::value_ptr(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f)));

	glm::vec3 worldLightPos(CalcLightPosition());
	
	glUniform3fv(UniformColorAndLightProgram.worldSpaceLightPosUnif, 1, glm::value_ptr(worldLightPos));

	g_pPlaneMesh->Render();

	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
}

void DrawUniformGround(glutil::MatrixStack &modelMatrix)
{
	glutil::PushStack push(modelMatrix);
	
	glm::mat4 worldToCamera = modelMatrix.Top();

	modelMatrix.SetIdentity();
	modelMatrix.Scale(glm::vec3(100.0f, 1.0f, 200.0f));

	glUseProgram(UniformColorAndLightProgram.theProgram);
	glUniformMatrix4fv(UniformColorAndLightProgram.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));

	glUniform3f(UniformColorAndLightProgram.ka, 0.2f, 0.2f, 0.2f);

	glUniform1f(UniformColorAndLightProgram.spotLight.cutoffUnif, 73.0);
	glUniform1f(UniformColorAndLightProgram.spotLight.exponentUnif, 3.0);

	glUniform3fv(UniformColorAndLightProgram.spotLight.directionUnif, 1, glm::value_ptr(car->reflectorDirection()));
	glUniform3fv(UniformColorAndLightProgram.spotLight.intensityUnif, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));

	SendReflectorPosition(UniformColorAndLightProgram.spotLight.positionLeftReflectorUnif, -0.2f, 0.2f, 0.5f);
	SendReflectorPosition(UniformColorAndLightProgram.spotLight.positionRightReflectorUnif, 0.2f, 0.2f, 0.5f);

	glUniformMatrix4fv(UniformColorAndLightProgram.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniformMatrix4fv(UniformColorAndLightProgram.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(worldToCamera));
	glUniform4f(UniformColorAndLightProgram.baseColorUnif, 0.62f, 0.62f, 0.27f, 1.0f);

	glUniform4fv(UniformColorAndLightProgram.lightIntensityUnif, 1, glm::value_ptr(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f)));

	glm::vec3 worldLightPos(CalcLightPosition());
	
	glUniform3fv(UniformColorAndLightProgram.worldSpaceLightPosUnif, 1, glm::value_ptr(worldLightPos));
	
	g_pPlaneMesh->Render();

	glUseProgram(0);
}

void DrawGround(glutil::MatrixStack &modelMatrix)
{
	DrawUniformGround(modelMatrix);
	DrawRoad(modelMatrix);
}

void DrawTetrahedron(glutil::MatrixStack &modelMatrix)
{
	glutil::PushStack push(modelMatrix);
	g_TetrahedronTimer.Update();

	glm::mat4 worldToCamera = modelMatrix.Top();

	modelMatrix.SetIdentity();
	modelMatrix.Translate(40.0f, 2.0f, 75.0f);
	modelMatrix.RotateY(360.0f * g_TetrahedronTimer.GetAlpha());
	modelMatrix.Translate(1.0f, 1.0f + sqrtf(2.0f) * 10, 1.0f);
	modelMatrix.Scale(10);
	modelMatrix.Rotate(glm::vec3(-0.707f, 0.0f, -0.707f), 54.735f);
	
	glUseProgram(ReflectorsAndLightProgram.reflectorsProgramData.theProgram);

	glUniform1f(ReflectorsAndLightProgram.reflectorsProgramData.shininess, 2.3f);
	glUniform3f(ReflectorsAndLightProgram.reflectorsProgramData.ks, 0.5f, 0.5f, 0.5f);
	glUniform3f(ReflectorsAndLightProgram.reflectorsProgramData.ka, 0.2f, 0.2f, 0.2f);
	glUniform3f(ReflectorsAndLightProgram.reflectorsProgramData.kd, 0.3f, 0.3f, 0.3f);

	glUniform1f(ReflectorsAndLightProgram.reflectorsProgramData.spotLight.cutoffUnif, 73.0f);
	glUniform1f(ReflectorsAndLightProgram.reflectorsProgramData.spotLight.exponentUnif, 3.0f);

	glUniform3fv(ReflectorsAndLightProgram.reflectorsProgramData.spotLight.directionUnif, 1, glm::value_ptr(car->reflectorDirection()));
	glUniform3fv(ReflectorsAndLightProgram.reflectorsProgramData.spotLight.intensityUnif, 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));

	SendReflectorPosition(ReflectorsAndLightProgram.reflectorsProgramData.spotLight.positionLeftReflectorUnif, -0.2f, 0.2f, -0.5f);
	SendReflectorPosition(ReflectorsAndLightProgram.reflectorsProgramData.spotLight.positionRightReflectorUnif, 0.2f, 0.2f, -0.5f);

	glUniformMatrix4fv(ReflectorsAndLightProgram.reflectorsProgramData.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniformMatrix4fv(ReflectorsAndLightProgram.reflectorsProgramData.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(worldToCamera));

	glUniform4fv(ReflectorsAndLightProgram.lightIntensityUnif, 1, glm::value_ptr(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f)));

	glm::vec3 worldLightPos(CalcLightPosition());
	
	glUniform3fv(ReflectorsAndLightProgram.worldSpaceLightPosUnif, 1, glm::value_ptr(worldLightPos));
	
	g_pTetrahedronMesh->Render();

	glUseProgram(0);
}

void DrawSphere(glutil::MatrixStack &modelMatrix)
{
	glutil::PushStack push(modelMatrix);

	g_SphereTimer.Update();
	float fCurrTimeThroughLoop = g_SphereTimer.GetAlpha();

	glm::vec3 pos(0.0f);

	pos.x = cosf(fCurrTimeThroughLoop * (3.14159f * 2.0f)) * g_fSphereRadius;
	pos.z = sinf(fCurrTimeThroughLoop * (3.14159f * 2.0f)) * g_fSphereRadius - 50.0f;
	pos.y = g_fSphereBaseHeight + sinf(fCurrTimeThroughLoop * (3.14159f * 2.0f)) * g_SphereDiffHeight;

	modelMatrix.Translate(pos);
	modelMatrix.Scale(10);

	glUseProgram(UniformColor.theProgram);
	glUniformMatrix4fv(UniformColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform4f(UniformColor.baseColorUnif, 0.8f, 0.8f, 0.7f, 1.0f);
	g_pSphereMesh->Render();

	glUseProgram(0);

}

void DrawLookAtPoint(glutil::MatrixStack &modelMatrix)
{
	glDisable(GL_DEPTH_TEST);
	glutil::PushStack push(modelMatrix);

	modelMatrix.SetIdentity();
	modelMatrix.Translate(glm::vec3(0.0f, 0.0f, -g_viewPole.GetView().radius));
		
	glUseProgram(UniformColor.theProgram);
	glUniformMatrix4fv(UniformColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
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

		DrawTetrahedron(modelMatrix);

		DrawSphere(modelMatrix);

		if(g_bDrawLookatPoint)
		{
			DrawLookAtPoint(modelMatrix);
		}

	}

	glutPostRedisplay();
	glutSwapBuffers();
}

