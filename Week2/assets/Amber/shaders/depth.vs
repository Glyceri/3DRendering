#version 330 core

// Input vertex data, different for all executions of this shader.
in vec3 vertex;
in vec3 normal;
in vec2 uv;

// Values that stay constant for the whole mesh.
uniform mat4 depthMVP;

void main(){
    //gl_Position =  depthMVP * vec4(vertex,1);
    gl_Position = depthMVP * vec4(vertex, 1.f);;
}