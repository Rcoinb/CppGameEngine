#version 330

layout(location = 0) in vec3 position;

out vec2 texCoord;

uniform mat4 modelMatrix;
uniform int useSubTex;
uniform float imageDim;
uniform vec2 subTexCor;
uniform vec2 subTexDim;

void main(void){
	gl_Position = modelMatrix * vec4(position.xy, 0.0, 1.0);
	
	float addX = 0;
	float addY = 0;
	float width = 2;
	float height = 2;
	
	if (useSubTex == 1) {
		addX = subTexCor.x / imageDim;
		addY = subTexCor.y / imageDim;
		width = (imageDim * 2f) / subTexDim.x;
		height = (imageDim * 2f) / subTexDim.y;
	}
	
	texCoord = vec2(addX + ((position.x + 1) / width), addY + ((position.y - 1) / -height));
}