#include "Variables.h"


Car::Car(GLfloat* data, GLuint size, GLuint verticesCount) : _data(data), _descriptionSize(size), _verticesCount(verticesCount), 
													_x(0.0f), _z(-80.0f), _scale(10.0f), 
													_degree(0.0f), _isEmptySpace(NULL)
{ 
	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);

	glGenBuffers(1, &this->_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
	glEnableVertexAttribArray(0);
	glBufferData(GL_ARRAY_BUFFER, _descriptionSize, _data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Car::Render()
{
	glBindVertexArray(_vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glDrawArrays(GL_TRIANGLES, 0, _verticesCount);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Car::move(GLfloat forward)
{
	GLfloat tmpx = _x + sin(_degree) * forward;
	GLfloat tmpz = _z + cos(_degree) * forward;
	
	if(_isEmptySpace(tmpx, tmpz))
	{
		_x = tmpx;
		_z = tmpz;
	}
}

void Car::turn(GLfloat degree)
{
	_degree += degree;
}

glm::vec3 Car::leftReflectorPosition()
{
	glm::vec4 baseReflectorPosition(0.1, 0.2, 0.5, 1);
	return glm::vec3(modelToWorldMatrix() * baseReflectorPosition);
}

glm::vec3 Car::rightReflectorPosition()
{
	glm::vec4 baseReflectorPosition(0.1, 0.2, 0.5, 1);
	return glm::vec3(modelToWorldMatrix() * baseReflectorPosition);
}

glm::vec3 Car::reflectorDirection()
{
	glutil::MatrixStack modelMatrix;

	modelMatrix.RotateY(glm::degrees(_degree));

	glm::vec4 modelReflectorDirection(0.0, 0.0, 1.0, 1.0);
	return glm::vec3(modelMatrix.Top() * modelReflectorDirection);
}

glm::mat4 Car::modelToWorldMatrix(glutil::MatrixStack &modelMatrix)
{
	glutil::PushStack push(modelMatrix);

	modelMatrix.Translate(_x, 0, _z);
	modelMatrix.Scale(_scale);
	modelMatrix.RotateY(glm::degrees(_degree));

	return modelMatrix.Top();
}


glm::mat4 Car::modelToWorldMatrix()
{
	glutil::MatrixStack modelMatrix;

	return modelToWorldMatrix(modelMatrix);
}

void Car::emptySpaceHandler(bool (*fun)(GLfloat x, GLfloat z))
{
	_isEmptySpace = fun;
}

static GLfloat car_description[] = 
{
	-0.25f, 0.05f, -0.5f,
	0.25f, 0.05f, -0.5f,
	-0.25f, 0.05f, 0.5f,
	
	0.25f, 0.05f, -0.5f,
	0.25f, 0.05f, 0.5f,
	-0.25f, 0.05f, 0.5f,

	0.25f, 0.05f, 0.5f,
	-0.25f, 0.05f, 0.5f,
	-0.25f, 0.35f, 0.5f,

	0.25f, 0.05f, 0.5f,
	-0.25f, 0.35f, 0.5f,
	0.25f, 0.35f, 0.5f,

	-0.25f, 0.35f, -0.5f,
	0.25f, 0.05f, -0.5f,
	0.25f, 0.35f, -0.5f,

	-0.25f, 0.35f, -0.5f,
	-0.25f, 0.05f, -0.5f,
	0.25f, 0.05f, -0.5f,

	-0.25f, 0.05f, 0.5f,
	-0.25f, 0.05f, -0.5f,
	-0.25f, 0.35f, -0.5f,

	-0.25f, 0.05f, 0.5f,
	-0.25f, 0.35f, -0.5f,
	-0.25f, 0.35f, 0.5f,

	0.25f, 0.05f, 0.5f,
	0.25f, 0.35f, -0.5f,
	0.25f, 0.05f, -0.5f,

	0.25f, 0.05f, 0.5f,
	0.25f, 0.35f, 0.5f,
	0.25f, 0.35f, -0.5f,

	-0.25f, 0.35f, -0.5f,
	0.25f, 0.35f, -0.5f,
	-0.25f, 0.35f, 0.5f,
	
	0.25f, 0.35f, -0.5f,
	0.25f, 0.35f, 0.5f,
	-0.25f, 0.35f, 0.5f,

	-0.25f, 0.35f, 0.2f, 
	-0.25f, 0.35f, 0.1f, 
	-0.25f, 0.5f, 0.1f, 

	-0.25f, 0.35f, -0.35f, 
	-0.25f, 0.35f, -0.4f, 
	-0.25f, 0.5f, -0.35f, 

	-0.25f, 0.35f, 0.1f, 
	-0.25f, 0.35f, -0.35f, 
	-0.25f, 0.5f, -0.35f, 

	-0.25f, 0.35f, 0.1f, 
	-0.25f, 0.5f, -0.35f,
	-0.25f, 0.5f, 0.1f, 
	
	0.25f, 0.35f, 0.2f, 
	0.25f, 0.5f, 0.1f, 
	0.25f, 0.35f, 0.1f, 

	0.25f, 0.35f, -0.35f, 
	0.25f, 0.5f, -0.35f, 
	0.25f, 0.35f, -0.4f, 

	0.25f, 0.35f, 0.1f, 
	0.25f, 0.5f, -0.35f,
	0.25f, 0.35f, -0.35f, 

	0.25f, 0.5f, -0.35f,
	0.25f, 0.35f, 0.1f, 
	0.25f, 0.5f, 0.1f, 
	
	0.25f, 0.35f, -0.4f, 
	0.25f, 0.5f, -0.35f,
	-0.25f, 0.35f, -0.4f, 

	0.25f, 0.5f, -0.35f, 
	-0.25f, 0.5f, -0.35f,
	-0.25f, 0.35f, -0.4f,

	0.25f, 0.5f, 0.1f,
	-0.25f, 0.5f, 0.1f,
	-0.25f, 0.5f, -0.35f,

	-0.25f, 0.5f, -0.35f,
	0.25f, 0.5f, -0.35f,
	0.25f, 0.5f, 0.1f,

	-0.25f, 0.5f, 0.1f,
	0.25f, 0.5f, 0.1f, 
	0.25f, 0.35f, 0.2f,

	0.25f, 0.35f, 0.2f,
	-0.25f, 0.35f, 0.2f,
	-0.25f, 0.5f, 0.1f
};

void Car::Initialize() 
{
	car = new Car(car_description, sizeof(car_description), COUNT(car_description) / 3);
}
