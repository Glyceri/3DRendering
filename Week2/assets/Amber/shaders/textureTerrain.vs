//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 	mvpMatrix;
uniform mat4    modelMat;

uniform sampler2D heightMap;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 texCoord; //make sure the texture coord is interpolated
out vec3 fNormal;
out vec3 fragPosition;

void main( void ){
    vec4 t = texture(heightMap, uv);
    float power = (t.r + t.g + t.b) / 3.0;
    float terrainY = -0.1f + power;

    vec3 newVert = vec3(vertex.x, vertex.y+terrainY, vertex.z);

    gl_Position = mvpMatrix * vec4(newVert, 1.f);
    fNormal = mat3(transpose(inverse(modelMat))) * normal;
	texCoord = uv;
    fragPosition = (modelMat * vec4(newVert, 1.f)).xyz;
}