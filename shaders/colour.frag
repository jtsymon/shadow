#version 330 core

out vec4 colour;

uniform vec4 colour_in;

void main() {
    colour = colour_in;
}
