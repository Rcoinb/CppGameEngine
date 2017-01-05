#version 330

const int MAX_LIGHT_COUNT = 16;

in vec3 normal0;
in vec2 texCoord0;
in vec3 worldPos0;
in vec3 cameraPos0;

uniform sampler2D sampler;
uniform vec4 mat_color;
uniform float mat_specularIntensity;
uniform float mat_specularPower;

uniform int l_count;

uniform samplerCube s_cubeMap[MAX_LIGHT_COUNT];
uniform float s_fplane[MAX_LIGHT_COUNT];
uniform vec3 l_position[MAX_LIGHT_COUNT];
uniform vec3 l_direction[MAX_LIGHT_COUNT];
uniform vec3 l_color[MAX_LIGHT_COUNT];
uniform float l_cutoff[MAX_LIGHT_COUNT];
uniform float l_ambient[MAX_LIGHT_COUNT];
uniform float l_range[MAX_LIGHT_COUNT];
uniform float l_attenConstant[MAX_LIGHT_COUNT];
uniform float l_attenLinear[MAX_LIGHT_COUNT];
uniform float l_attenExponent[MAX_LIGHT_COUNT];



vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1), 
   vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
   vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
   vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

float ShadowCalculation(samplerCube shadowCubeMap, vec3 fragPos, vec3 lightPos, float far_plane)
{
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);
    float shadow = 0.0;
    float bias = 0.000001;
    int samples = 20;
    float viewDistance = length(cameraPos0 - fragPos);
    float diskRadius = 0.01;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(shadowCubeMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // Undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
	
	if (length(fragPos - lightPos) > far_plane) shadow = 0;
	
    return shadow;
}

vec4 calcPointLight(vec3 normal, int light)
{
    vec3 lightDir = worldPos0 - l_position[light];
    float distanceToPoint = length(lightDir);

    lightDir = normalize(lightDir);   
    
    float diffuseFactor = dot(normal, -lightDir);
    
    vec4 difColor = vec4(l_color[light], 1.0) * l_ambient[light];
	vec4 specColor = vec4(0, 0, 0, 0);
    
    if (diffuseFactor > 0) 
	{
		//diffuse lighting
        difColor = difColor * diffuseFactor;
        
        //specular lighting
        if (mat_specularIntensity > 0){
            vec3 vertexToEye = normalize(cameraPos0 - worldPos0);
            vec3 lightReflect = normalize(reflect(lightDir, normal));
            float specularFactor = dot(vertexToEye, lightReflect);
            if (specularFactor > 0) {
                specularFactor = pow(specularFactor, mat_specularPower);
                specColor = vec4(l_color[light].xyz * mat_specularIntensity * specularFactor, 1.0f);
            }
        }
	}
    else difColor = difColor * 0.0;
    

    float attenuation = 1 / (l_attenConstant[light] + l_attenLinear[light] * distanceToPoint + l_attenExponent[light] * distanceToPoint * distanceToPoint);  
    
	float shadow = 1 - ShadowCalculation(s_cubeMap[light], worldPos0, l_position[light], s_fplane[light]);
	
    difColor = difColor * attenuation;
	specColor = specColor * attenuation;  
	
	difColor = difColor * shadow;
	specColor = specColor * shadow;
	
    return difColor + specColor;
}

vec4 calcSpotLight(vec3 normal, int light)
{
    vec3 LightToPixel = normalize(worldPos0 - l_position[light]);
    float SpotFactor = dot(LightToPixel, l_direction[light]);
    vec4 Color = calcPointLight(normal, light);
    
    if (SpotFactor > l_cutoff[light]) return Color * (1.0 - (1.0 - SpotFactor) * 1.0 / (1.0 - l_cutoff[light]));
    else return vec4(0, 0, 0, 0);
}


void main()
{

    vec4 lightColor = vec4(0, 0, 0, 0);
	
	for(int i = 0; i < l_count; i ++) {
        lightColor += calcSpotLight(normalize(normal0), i);
    }
	
	gl_FragColor = (texture2D(sampler, texCoord0) + mat_color) * lightColor;
}
