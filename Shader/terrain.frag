#version 460 core

// -------------- Variable from .tes --------------
in float Height;
in vec3 normal;
in vec2 TextureCoord;
in vec3 FragPos;
// ------------------------------------------------

// -------------- Terrain Layer --------------
uniform sampler2D u_GrassTexture;
uniform sampler2D u_SnowTexture;
uniform sampler2D u_GroundTexture;
uniform sampler2D u_WaterTexture;
uniform sampler2D u_SandTexture;
//--------------------------------------------

// -------------- Lighting Variable --------------
uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform vec3 u_ViewPos;
const float ambientStrength = 0.1f;
const float shininess = 16.0f;
//------------------------------------------------

// -------------- Fog Variable --------------
uniform bool u_IsFogActive;
uniform float u_MinFogDistance;
uniform float u_MaxFogDistance;
uniform float u_Density;
uniform float u_Gradient;
uniform vec3 u_FogColor;
// ------------------------------------------

uniform bool u_IsWireFrame;

out vec4 FragColor;

vec3 CalculateLight()
{
	vec3 lightDir = vec3(0.2f, 1.0f, 0.3f);
	//vec3 lightDir = normalize(u_LightPos - FragPos);
	vec3 viewDir = normalize(u_ViewPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	vec3 ambient = ambientStrength * u_LightColor;
	
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * u_LightColor;

	float spec = pow(max(dot(normal, halfwayDir), 0.0f), shininess);
	vec3 specular = spec * u_LightColor;

	return ambient + diffuse;
}
float CalculateFogFactor()
{
	float dist = length(u_ViewPos - FragPos);
	float fogFactor = (u_MaxFogDistance - dist) / (u_MaxFogDistance - u_MinFogDistance);
	//fogFactor = exp(-pow(dist * u_Density, u_Gradient));

	return smoothstep(0.0f, 1.0f, clamp(fogFactor, 0.0f, 1.0f));
}

void main()
{
	float h = Height;
	vec4 result = vec4(1.0f);

	if(!u_IsWireFrame)
	{
		if(h > 0.9f)
		{
			result = texture(u_SnowTexture, TextureCoord);
		}
		else if(h > 0.7f)
		{
			result = mix(texture(u_GrassTexture, TextureCoord), texture(u_SnowTexture, TextureCoord), smoothstep(0.7f, 0.9f, h));
		}
		else if(h > 0.5f)
		{	
			result = mix(texture(u_GroundTexture, TextureCoord), texture(u_GrassTexture, TextureCoord), smoothstep(0.5f, 0.7f, h));
		}
		else if(h > 0.3f)
		{	
			result = mix(texture(u_SandTexture, TextureCoord), texture(u_GroundTexture, TextureCoord), smoothstep(0.3f, 0.5f, h));
		}
		else
		{
			result = mix(texture(u_WaterTexture, TextureCoord), texture(u_SandTexture, TextureCoord), smoothstep(0.0f, 0.3f * 2.0f, h));
		}

		if(u_IsFogActive)
		{
			float fog_factor = CalculateFogFactor();
			FragColor = mix(vec4(mix(u_FogColor, CalculateLight(), fog_factor), 1.0f), result, fog_factor);
		}
		else
		{
			FragColor = result * vec4(CalculateLight(), 1.0f);
		}
	}
	else
	{
		FragColor = vec4(mix(vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), smoothstep(0.0f, 1.0f, h)), 1.0f);
	}
}