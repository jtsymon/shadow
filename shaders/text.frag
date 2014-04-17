#version 330

out vec4 colour;

in vec2 UV;
uniform vec4 colour_in;
uniform sampler2D font;

int main() {
    colour = vec4(colour_in.xyz, colour_in.w * texture2D(font, UV).a);
}