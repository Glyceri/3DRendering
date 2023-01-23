//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 	mvpMatrix;
uniform mat4    modelMat;
uniform mat4    lightMat;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 texCoord; //make sure the texture coord is interpolated
out vec3 fNormal;
out vec3 fragPosition;
out vec4 FragPosLightSpace;

void main( void ){
    gl_Position = mvpMatrix * vec4(vertex, 1.f);
    fNormal = mat3(transpose(inverse(modelMat))) * normal;
	texCoord = uv;
    fragPosition = (modelMat * vec4(vertex, 1.f)).xyz;
    FragPosLightSpace = lightMat * vec4(vertex, 1.f);
}