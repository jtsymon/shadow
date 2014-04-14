#version 330 core

// in vec2 UV;
// in vec4 gl_Color;
out vec4 colour;

uniform vec4 colour_in;

void main() {
    colour = colour_in;
}
