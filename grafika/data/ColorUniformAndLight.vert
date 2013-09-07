#version 330

layout(location = 0) in vec4 position;
layout(location = 2) in vec4 normal;

out vec4 diffuseColor;
out vec3 worldSpacePosition;
out vec3 vertexNormal;

uniform mat4 modelToWorldMatrix;
uniform mat4 worldToCameraMatrix;
uniform mat4 cameraToClipMatrix;

void main()
{
	worldSpacePosition = vec3(modelToWorldMatrix * vec4(position));
    gl_Position = cameraToClipMatrix * worldToCameraMatrix * vec4(worldSpacePosition, 1.0);
    vertexNormal = vec3(normal);
}
