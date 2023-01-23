//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330  // for glsl version (12 is for older versions , say opengl 2.1

//Only needs to pass one time!
uniform sampler2D diffuseTexture;
uniform vec3 camPos;
uniform int shinyness;

//Needs to pass for all!
uniform vec3 directionalLightColour;
uniform float lampStrength;
uniform vec3 lightPos;
uniform vec3 lightDir;
uniform int lightType;





in vec3 fNormal;
in vec2 texCoord;
in vec3 fragPosition;
out vec4 fragment_color;

float CalculateAttenuation(){
	float dist = distance(lightPos, fragPosition);
	float constant = 1;
	float linear = 0;
	float quadratic = 0.02f;
	float attenuation = (constant + linear * dist+ quadratic *pow(dist, 2.0));
	return attenuation;
}

vec3 HandleAmbient(){
	return directionalLightColour * lampStrength;
}

vec3 HandlePoint(){
	float d = max(dot(normalize(fNormal),normalize(lightPos - fragPosition)), 0);

	vec3 diffuseTerm =  d * directionalLightColour * lampStrength;
	return diffuseTerm;
}

vec3 HandleSpecular(){
	vec3 specularColour = vec3(1,1,1);
	vec3 reflection = reflect(normalize(lightPos - fragPosition), normalize(fNormal));
	vec3 viewDir = normalize(fragPosition - camPos);
	float specMap = max(dot(reflection, viewDir), 0);
	float shinySpecMap = pow(specMap, shinyness);
	vec3 finalCol = shinySpecMap * directionalLightColour * specularColour;
	return finalCol;
}

vec3 HandleAsPoint(){
 	vec3 diffuseTerm  = HandlePoint();
	vec3 specularTerm = HandleSpecular();
	return (diffuseTerm + specularTerm) / CalculateAttenuation();
}

vec3 HandleAsSpot(){
	vec3 lPix = normalize(fragPosition - lightPos);
	float spotFac = dot(lPix, normalize(lightDir));

	if(spotFac > 0.95f){
		return HandleAsPoint();
	}
	return vec3(0, 0, 0);
}

vec3 HandleAsDirectional(){

	float lightStrength =  1-max(dot(normalize(fNormal), normalize(lightDir)),0);
	return HandleAmbient() * lightStrength;
}

void main (void) {
	vec3 textureC = texture(diffuseTexture, texCoord).xyz;
	vec3 outcome = vec3(0,0,0);
	if(lightType == 0) outcome = HandleAsPoint();
	if(lightType == 1) outcome = HandleAsSpot();
	if(lightType == 2) outcome = HandleAsDirectional();
    fragment_color = vec4 (outcome * textureC, 1);
}
