#pragma once

class Car
{
	GLuint _vao;
	GLuint _vbo;

	GLfloat* _data;
	GLuint _descriptionSize;
	GLuint _verticesCount;

	GLfloat _z;
	GLfloat _x;
	GLfloat _scale;
	GLfloat _degree;

	bool (*_isEmptySpace)(GLfloat x, GLfloat z);

public:
	Car(GLfloat* data, GLuint descriptionSize, GLuint verticesCount);

	GLfloat x();
	GLfloat z();
	GLfloat scale();
	GLfloat degree();

	void emptySpaceHandler(bool (*fun)(GLfloat x, GLfloat z));
	static void Initialize();
	void Render();
	void move(GLfloat forward);
	void turn(GLfloat degree);
	glm::mat4x4 modelToWorldMatrix(glutil::MatrixStack &modelMatrix);
	glm::mat4x4 modelToWorldMatrix();
	glm::vec3 reflectorDirection();
};
