#version 330


in vec3 worldSpacePosition;
in vec3 vertexNormal;
in vec4 diffuseColor;

struct SpotLightInfo {
    vec4 positionLeftReflector;			// Position in world coords
	vec4 positionRightReflector;			// Position in world coords
    vec3 intensity;
    vec3 direction;				// Direction of the spotlight in model coords.
    float exponent;				// Angular attenuation exponent
    float cutoff;				// Cutoff angle (between 0 and 90)
};

uniform SpotLightInfo Spot;
uniform vec3 Kd;				// Diffuse reflectivity
uniform vec3 Ka;				// Ambient reflectivity
uniform vec3 Ks;				// Specular reflectivity
uniform float Shininess;		// Specular shininess factor
uniform vec3 worldSpaceLightPos;
uniform vec4 lightIntensity;

out vec4 outputColor;

vec3 adsWithSpotlight()
{
	vec3 ambient = Spot.intensity * Ka;
	vec3 sl = normalize(vec3(Spot.positionLeftReflector) - worldSpacePosition);
	vec3 sr = normalize(vec3(Spot.positionRightReflector) - worldSpacePosition);
    vec3 spotDir = normalize(Spot.direction);
    float anglel = acos(dot(-sl, spotDir));
	float angler = acos(dot(-sr, spotDir));
    float cutoff = radians(clamp(Spot.cutoff, 0.0, 90.0));

	vec3 lightDir = normalize(worldSpaceLightPos - worldSpacePosition);
	float cosAngIncidence = dot(normalize(vertexNormal), lightDir);
	cosAngIncidence = clamp(cosAngIncidence, 0, 1);
	vec3 baseLight = vec3(lightIntensity * cosAngIncidence) + ambient;
    
    if (anglel < cutoff || angler < cutoff) {
		vec3 v = normalize(vec3(-worldSpacePosition));
		vec3 tmpColorl;
		vec3 tmpColorr;

		if(anglel < cutoff) {
			float spotFactor = pow(dot(-sl, spotDir), Spot.exponent);
			vec3 hl = normalize(v + sl);
			tmpColorl =  spotFactor * Spot.intensity * (Kd * max(dot(sl, vertexNormal), 0.0) + Ks * pow(max(dot(hl, vertexNormal), 0.0), Shininess));
		}

		if(angler < cutoff) {
			float spotFactor = pow(dot(-sr, spotDir), Spot.exponent);
			vec3 hr = normalize(v + sr);
			tmpColorr =  spotFactor * Spot.intensity * (Kd * max(dot(sr, vertexNormal), 0.0) + Ks * pow(max(dot(hr, vertexNormal), 0.0), Shininess));
		}

		return baseLight + tmpColorl + tmpColorr;
    } else {
		return baseLight;
    }
}

void main()
{
    outputColor = vec4(adsWithSpotlight(), 1.0f) * diffuseColor;
}