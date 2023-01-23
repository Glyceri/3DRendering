//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 	mvpMatrix;
uniform mat4    modelMat;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 texCoord; //make sure the texture coord is interpolated
out vec3 fNormal;
out vec3 fragPosition;

void main( void ){
    gl_Position = mvpMatrix * vec4(vertex, 1.f);
    fNormal = (transpose(inverse(modelMat)) * vec4(normal, 0.0)).xyz;
	texCoord = uv;
    fragPosition = (modelMat * vec4(vertex, 1.f)).xyz;
}
