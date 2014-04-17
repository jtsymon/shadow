#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 vertexPosition_modelspace;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

void main(){
    gl_Position =  vec4(vertexPosition_modelspace.xy, 1, 1);
    UV = vec2(vertexPosition_modelspace.z, -vertexPosition_modelspace.w);
}
