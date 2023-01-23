//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330  // for glsl version (12 is for older versions , say opengl 2.1

//Only needs to pass one time!
uniform sampler2D diffuseTexture;
uniform vec3 camPos;
uniform int shinyness;

//Needs to pass for all!
struct LightElement {
	int lightType;
	vec3 lightColour;
	vec3 lightPosition;
	vec3 lightDirection;
	float lightPower;

	vec3 extraAttributes;
};
#define LIGHTCOUNT 32
uniform LightElement lightElements[LIGHTCOUNT];

in vec3 fNormal;
in vec2 texCoord;
in vec3 fragPosition;
out vec4 fragment_color;

float CalculateAttenuation(vec3 lightPos, vec3 ex){
	float dist = distance(lightPos, fragPosition);
	float constant = ex.x;
	float linear = ex.y;
	float quadratic = ex.z;
	float attenuation = (constant + linear * dist+ quadratic *pow(dist, 2.0));
	return attenuation;
}

vec3 HandleAmbient(vec3 baseColour, float lampStrength){
	return baseColour * lampStrength;
}

vec3 HandlePoint(vec3 lightPos, vec3 baseColour, float lightPower){
	float d = max(dot(normalize(fNormal),normalize(lightPos - fragPosition)), 0);

	vec3 diffuseTerm =  d * baseColour * lightPower;
	return diffuseTerm;
}

vec3 HandleSpecular(vec3 lightPos, vec3 baseColour){
	vec3 specularColour = vec3(1,1,1);
	vec3 reflection = reflect(normalize(lightPos - fragPosition), normalize(fNormal));
	vec3 viewDir = normalize(fragPosition - camPos);
	float specMap = max(dot(reflection, viewDir), 0);
	float shinySpecMap = pow(specMap, shinyness);
	vec3 finalCol = shinySpecMap * baseColour * specularColour;
	return finalCol;
}

vec3 HandleAsPoint(vec3 lightPos, vec3 baseColour, float lightPower){
 	vec3 diffuseTerm  = HandlePoint(lightPos, baseColour, lightPower);
	vec3 specularTerm = HandleSpecular(lightPos, baseColour);
	return (diffuseTerm + specularTerm);
}

vec3 HandleAsSpot(vec3 lightPos, vec3 lightDir, vec3 baseColour, float lightPower, float angle, float falloff, float maxDistance){
	vec3 dir = fragPosition - lightPos;
	vec3 lPix = normalize(dir);
	float spotFac = dot(lPix, normalize(lightDir));

	float intensity = clamp((spotFac - falloff) / (angle -falloff), 0.0, 1.0);

	if(spotFac > falloff && length(dir) <= maxDistance){
		return HandleAsPoint(lightPos, baseColour, lightPower) * intensity;
	}
	return vec3(0, 0, 0);
}

vec3 HandleAsDirectional(vec3 lightDir, float lampStrength, vec3 baseColour){

	float lightStrength =  max(dot(normalize(fNormal), normalize(lightDir)),0);
	return HandleAmbient(baseColour, lampStrength) * lightStrength;
}

vec3 HandleAsAmb(float power, vec3 lightColour){
	return power * lightColour;
}

void main (void) {
	vec3 textureC = texture(diffuseTexture, texCoord).xyz;
	vec3 calculatedLight = vec3(0,0,0);
	for(int i = 0; i < LIGHTCOUNT; i++){
		LightElement el = lightElements[i];
		if(el.lightType < 0) continue;
		vec3 outcome = vec3(0,0,0);
		if(el.lightType == 0) outcome = HandleAsPoint(el.lightPosition, el.lightColour, el.lightPower) / CalculateAttenuation(el.lightPosition, el.extraAttributes);
		if(el.lightType == 1) outcome = HandleAsSpot(el.lightPosition, el.lightDirection, el.lightColour, el.lightPower, el.extraAttributes.x, el.extraAttributes.y, el.extraAttributes.z);
		if(el.lightType == 2) outcome = HandleAsDirectional(el.lightDirection, el.lightPower, el.lightColour);
		if(el.lightType == 3) outcome = HandleAsAmb(el.lightPower, el.lightColour);
		calculatedLight += outcome;
	}
	//fragment_color = vec4 (calculatedLight, 1);
    fragment_color = vec4 (calculatedLight * textureC, 1);
}
