#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 inDiffuseColor;
layout(location = 2) in vec3 normal;

out vec4 diffuseColor;
out vec3 vertexNormal;
out vec3 worldSpacePosition;

uniform mat4 modelToWorldMatrix;
uniform mat4 worldToCameraMatrix;
uniform mat4 cameraToClipMatrix;

void main()
{
	worldSpacePosition = vec3(modelToWorldMatrix * vec4(position, 1.0));
    gl_Position = cameraToClipMatrix * worldToCameraMatrix * vec4(worldSpacePosition, 1.0);
	vertexNormal = normal;
	diffuseColor = inDiffuseColor;
}

