#include "Variables.h"

void reshape (int w, int h)
{
	glutil::MatrixStack persMatrix;
	persMatrix.Perspective(45.0f, (w / (float)h), g_fzNear, g_fzFar);

	glUseProgram(UniformColor.theProgram);
	glUniformMatrix4fv(UniformColor.cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(persMatrix.Top()));
	glUseProgram(ColorProvided.theProgram);
	glUniformMatrix4fv(ColorProvided.cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(persMatrix.Top()));
	glUseProgram(UniformColorAndLightProgram.theProgram);
	glUniformMatrix4fv(UniformColorAndLightProgram.cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(persMatrix.Top()));
	glUseProgram(ReflectorsAndLightProgram.reflectorsProgramData.theProgram);
	glUniformMatrix4fv(ReflectorsAndLightProgram.reflectorsProgramData.cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(persMatrix.Top()));
	glUseProgram(0);

	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glutPostRedisplay();
}