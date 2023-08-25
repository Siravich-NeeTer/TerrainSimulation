#include "Shader/noise.util"

in vec3 FragPos;

out vec4 FragColor;

void main()
{
	float depth = PerlinNoise(FragPos.xz, 1.0f, 4, 0.5f, 0.5f, 0, true);	

	FragColor = vec4(mix(vec3(0.0f, 0.0f, 0.543f), vec3(0.0f, 0.746f, 1.0f), depth), 0.5f);
	//FragColor = vec4(vec3(depth), 1.0f);
}