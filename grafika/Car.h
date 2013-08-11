#pragma once

class Car
{
	GLuint _vao;
	GLuint _vbo;

	GLfloat* _data;
	GLuint _size;
	GLuint _count;

	GLfloat _z;
	GLfloat _x;
	GLfloat _scale;
	GLfloat _degree;

	bool (*_isEmptySpace)(GLfloat x, GLfloat z);
public:
	Car(GLfloat* data, GLuint size, GLuint count);
	void emptySpaceHandler(bool (*fun)(GLfloat x, GLfloat z));
	static void Initialize();
	void Render();
	void move(GLfloat forward);
	void turn(GLfloat degree);
	glm::mat4x4 modelToWorldMatrix(glutil::MatrixStack &modelMatrix);
};
