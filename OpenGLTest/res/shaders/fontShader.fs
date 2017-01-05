#version 330

in vec2 texCoord0;

out vec4 outputColor;

uniform sampler2D fontTexture;

uniform vec4 color;

void main(void){
    outputColor = texture(fontTexture, texCoord0);
    outputColor = outputColor * color;
}