#include "Variables.h"


using Framework::Timer;

glm::mat4 CalcWorldToCameraMatrix(const glm::vec3 &cameraPt, const glm::vec3 &lookPt, const glm::vec3 &upPt)
{
	glm::vec3 lookDir = glm::normalize(lookPt - cameraPt);
	glm::vec3 upDir = glm::normalize(upPt);

	glm::vec3 rightDir = glm::normalize(glm::cross(lookDir, upDir));
	glm::vec3 perpUpDir = glm::cross(rightDir, lookDir);

	glm::mat4 rotMat(1.0f);
	rotMat[0] = glm::vec4(rightDir, 0.0f);
	rotMat[1] = glm::vec4(perpUpDir, 0.0f);
	rotMat[2] = glm::vec4(-lookDir, 0.0f);

	rotMat = glm::transpose(rotMat);

	glm::mat4 transMat(1.0f);
	transMat[3] = glm::vec4(-cameraPt, 1.0f);

	return rotMat * transMat;
}

glm::vec3 ResolveCamPosition()
{
	glutil::MatrixStack tempMat;

	float phi = Framework::DegToRad(g_sphereCamRelPos.x);
	float theta = Framework::DegToRad(g_sphereCamRelPos.y + 90.0f);

	float fSinTheta = sinf(theta);
	float fCosTheta = cosf(theta);
	float fCosPhi = cosf(phi);
	float fSinPhi = sinf(phi);

	glm::vec3 dirToCamera(fSinTheta * fCosPhi, fCosTheta, fSinTheta * fSinPhi);
	return (dirToCamera * g_sphereCamRelPos.z) + g_camTarget;
}

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

void DrawCar(glutil::MatrixStack &modelMatrix)
{
	glm::mat4 carModelMatrix = car->modelToWorldMatrix(modelMatrix);

	glUseProgram(UniformColor.theProgram);
	glUniformMatrix4fv(UniformColor.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(carModelMatrix));
	glUniform4f(UniformColor.baseColorUnif, 0.2f, 0.2f, 0.7f, 1.0f);
	car->Render();
	
	glUseProgram(0);
}

glm::vec4 CalcLightPosition()
{
	float fCurrTimeThroughLoop = g_LightTimer.GetAlpha();

	glm::vec4 ret(0.0f, g_fLightHeight, 0.0f, 1.0f);

	ret.x = cosf(fCurrTimeThroughLoop * (3.14159f * 2.0f)) * g_fLightRadius;
	ret.z = sinf(fCurrTimeThroughLoop * (3.14159f * 2.0f)) * g_fLightRadius;

	return ret;
}

void DrawShip(glutil::MatrixStack &modelMatrix)
{
	glutil::PushStack push(modelMatrix);

	modelMatrix.Translate(glm::vec3(0.0f, 0.0f, 50.0f));
	modelMatrix.Scale(3);
	modelMatrix.Rotate(glm::vec3(0, 1, 0), -34);

	g_LightTimer.Update();
	glm::vec4 lightPosModelSpace = CalcLightPosition();

	glUseProgram(ShipProgram.theProgram);
	glUniform3fv(ShipProgram.modelSpaceLightPosUnif, 1, glm::value_ptr(lightPosModelSpace));
	glUniformMatrix4fv(ShipProgram.modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelMatrix.Top()));
	glUniform4f(ShipProgram.lightIntensityUnif, 0.8f, 0.8f, 0.8f, 1.0f);
	glUniform4f(ShipProgram.ambientIntensityUnif, 0.2f, 0.2f, 0.2f, 1.0f);
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

void SendWorldToCameraMatrix(glm::vec3 &camPos)
{
	glutil::MatrixStack camMatrix;
	camMatrix.SetMatrix(CalcWorldToCameraMatrix(camPos, g_camTarget, glm::vec3(0.0f, 1.0f, 0.0f)));

	glUseProgram(UniformColor.theProgram);
	glUniformMatrix4fv(UniformColor.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(camMatrix.Top()));
	glUseProgram(ObjectColor.theProgram);
	glUniformMatrix4fv(ObjectColor.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(camMatrix.Top()));
	glUseProgram(UniformColorTint.theProgram);
	glUniformMatrix4fv(UniformColorTint.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(camMatrix.Top()));
	glUseProgram(ShipProgram.theProgram);
	glUniformMatrix4fv(ShipProgram.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(camMatrix.Top()));
	glUseProgram(ObjectColor.theProgram);
	glUniformMatrix4fv(ObjectColor.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0))); // vertex is already in camera space
	glUseProgram(0);
}

void DrawLookAtPoint(glutil::MatrixStack &modelMatrix, glm::vec3 &camPos)
{
	glDisable(GL_DEPTH_TEST);
	glutil::PushStack push(modelMatrix);

	glm::vec3 cameraAimVec = g_camTarget - camPos;
	modelMatrix.Translate(0.0f, 0.0, -glm::length(cameraAimVec));
		
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
		glm::vec3 &camPos = ResolveCamPosition();

		SendWorldToCameraMatrix(camPos);

		glutil::MatrixStack modelMatrix;

		DrawGround(modelMatrix);
		
		DrawForest(modelMatrix);

		DrawShip(modelMatrix);

		DrawCar(modelMatrix);

		if(g_bDrawLookatPoint)
		{
			DrawLookAtPoint(modelMatrix, camPos);
		}

	}

	glutPostRedisplay();
	glutSwapBuffers();
}

