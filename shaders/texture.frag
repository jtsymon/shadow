#version 330 core

in vec2 UV;
out vec4 colour;

uniform sampler2D texture;
uniform vec4 colour_in;

void main(){
    colour = colour_in * texture2D(texture, UV);
}
