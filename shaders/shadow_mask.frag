#version 330 core

in vec2 UV;
in vec4 gl_Color;
out vec3 color;

uniform sampler2D renderedTexture;

void main(){
    color = texture2D(renderedTexture, UV).rgb;
}
