#version 330

in vec2 texCoord;

out vec4 outputColor;

uniform sampler2D guiTexture;
uniform vec4 color;

void main(void){
    outputColor = texture(guiTexture, texCoord);
    outputColor = outputColor * color;
}