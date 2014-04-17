#version 330

layout(location = 0) in vec4 vertexPosition_modelspace;
out vec2 UV;

void main() {
    gl_position = vec4(vertexPosition_modelspace.xy, 1, 1);
    UV = (vertexPosition_modelspace.zw + vec2(1, 1)) / 2.0;
}