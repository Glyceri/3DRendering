//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330  // for glsl version (12 is for older versions , say opengl 2.1

//Only needs to pass one time!
#define DIFFUSECOUNT 4

uniform sampler2D splatMap;
uniform sampler2D heightMap;
uniform sampler2D diffuseMaps[DIFFUSECOUNT];
uniform vec3 camPos;
uniform int shinyness;
uniform mat4 modelMat;


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

vec3 usedNormal;

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
	float d = max(dot(normalize(usedNormal),normalize(lightPos - fragPosition)), 0);

	vec3 diffuseTerm =  d * baseColour * lightPower;
	return diffuseTerm;
}

vec3 HandleSpecular(vec3 lightPos, vec3 baseColour){
	vec3 specularColour = vec3(1,1,1);
	vec3 reflection = reflect(normalize(lightPos - fragPosition), normalize(usedNormal));
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

float lum(vec3 pix){
	return (pix.r + pix.g + pix.b) / 3.0f;
}
float lum(vec4 pix){
	return lum(pix.xyz);
}

vec3 HandleAsDirectional(vec3 lightDir, float lampStrength, vec3 baseColour){
	float lightStrength = max(dot(normalize(usedNormal), normalize(lightDir)), 0.0f);
	return HandleAmbient(baseColour,lampStrength) * lightStrength;
}

vec3 mixedSplat(vec4 splatTex){
	vec3 curTex = vec3(0,0,0);
	for(int i = 0; i < 4; i++){
		curTex += texture(diffuseMaps[i], texCoord).xyz * splatTex[i];
	}
	return curTex;
}

vec3 HandleAsAmb(float power, vec3 lightColour){
	return power * lightColour;
}

vec3 normalCalc(sampler2D heightmap){
	vec2 texSize = textureSize(heightMap, 0);
	float stepX = 0.1f;
	float stepY = 0.1f;
	vec4 curPix = texture(heightMap, texCoord);
	vec3 curPoint = vec3(texCoord.x,lum(curPix.xyz),texCoord.y);
	vec3 ul = vec3(texCoord.x, lum(texture(heightMap, texCoord + vec2(0, stepY)).xyz), texCoord.y + stepY);
	vec3 rl = vec3(texCoord.x + stepX, lum(texture(heightMap, texCoord + vec2(+stepX, 0)).xyz), texCoord.y);
	
	
	vec3 crossed = cross(normalize(rl - curPoint), normalize(ul - curPoint));
	return transpose(inverse(mat3(modelMat))) * normalize(crossed) ;
	//return normalize(crossed);
}

const float offs = 0.001f;

vec3 newNormalCalc(sampler2D heightMap){
	float top = lum(texture(heightMap, texCoord + vec2(0.0, offs)));
	float bot = lum(texture(heightMap, texCoord + vec2(0.0, -offs)));
	float left = lum(texture(heightMap, texCoord + vec2(-offs, 0.0)));
	float right = lum(texture(heightMap, texCoord + vec2(offs, 0.0)));
	vec3 n = vec3(0,0,0);
	n.x = left - right;
	n.y = bot - top;
	n.z = 2.0;
	return normalize(n);
}

void main (void) {
	vec4 splatTex = texture(splatMap, texCoord);
	vec3 textureC = mixedSplat(splatTex);

	usedNormal = normalCalc(heightMap);
	//textureC = newNormalCalc(heightMap);

	vec3 calculatedLight = vec3(0,0,0);
	for(int i = 0; i < LIGHTCOUNT; i++){

		LightElement el = lightElements[i];
		if(el.lightType < 0) continue;
		vec3 outcome = vec3(0,0,0);
		if(el.lightType == 0) outcome = HandleAsPoint(el.lightPosition, el.lightColour, el.lightPower) / CalculateAttenuation(el.lightPosition, el.extraAttributes);
		if(el.lightType == 1) outcome = HandleAsSpot(el.lightPosition, el.lightDirection, el.lightColour, el.lightPower, el.extraAttributes.x, el.extraAttributes.y, el.extraAttributes.z);
		if(el.lightType == 2) outcome = HandleAsDirectional(el.lightDirection, el.lightPower, el.lightColour) ;
		if(el.lightType == 3) outcome = HandleAsAmb(el.lightPower, el.lightColour);
		calculatedLight += outcome ; 
	}

	//fragment_color = vec4(usedNormal + calculatedLight, 1);
    fragment_color = vec4 (calculatedLight * textureC , 1) ;
}