#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 texCoord0;

uniform mat4 modelMatrix;

void main(void){
	gl_Position = modelMatrix * vec4(position.x, position.y, 0.0, 1.0);
	
	texCoord0 = texCoord;
}