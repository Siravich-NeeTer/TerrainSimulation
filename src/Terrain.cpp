#include "Terrain.h"

Terrain::Terrain(int width, int height)
	: m_TerrainShader("TerrainShader")
{
	InitShader();
	InitTexture();

	const float RESOLUTION = 0.5f;

	for (int r = 0; r < res; ++r)
	{
		for (int c = 0; c < res; ++c)
		{
			m_Vertices.emplace_back(width * r / (float)res);	// vertex1.x
			m_Vertices.emplace_back(0.0f);						// vertex1.y
			m_Vertices.emplace_back(height * c / (float)res);	// vertex1.z
			m_Vertices.emplace_back(0.0f);						// vertex1.u
			m_Vertices.emplace_back(0.0f);						// vertex1.v

			m_Vertices.emplace_back(width * (r + 1) / (float)res);	// vertex2.x
			m_Vertices.emplace_back(0.0f);							// vertex2.y
			m_Vertices.emplace_back(height * c / (float)res);		// vertex2.z
			m_Vertices.emplace_back(width / res * RESOLUTION);					// vertex2.u
			m_Vertices.emplace_back(0.0f);							// vertex2.v

			m_Vertices.emplace_back(width * r / (float)res);		// vertex3.x
			m_Vertices.emplace_back(0.0f);							// vertex3.y
			m_Vertices.emplace_back(height * (c + 1) / (float)res);	// vertex3.z
			m_Vertices.emplace_back(0.0f);							// vertex3.u
			m_Vertices.emplace_back(height / res * RESOLUTION);					// vertex3.v

			m_Vertices.emplace_back(width * (r + 1) / (float)res);	// vertex4.x
			m_Vertices.emplace_back(0.0f);							// vertex4.y
			m_Vertices.emplace_back(height * (c + 1) / (float)res);	// vertex4.z
			m_Vertices.emplace_back(width / res * RESOLUTION);					// vertex4.u
			m_Vertices.emplace_back(height / res * RESOLUTION);					// vertex4.v

		}
	}

	m_TerrainVAO.Bind();
	m_TerrainVBO.BufferData(sizeof(float) * m_Vertices.size(), m_Vertices.data(), false);

	m_TerrainVAO.Attribute(m_TerrainVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), 0);
	m_TerrainVAO.Attribute(m_TerrainVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), 2 * sizeof(float));

	glPatchParameteri(GL_PATCH_VERTICES, NUM_PATCH_PTS);
}
Terrain::~Terrain()
{
	m_TerrainVAO.Delete();
	m_TerrainVBO.Delete();
}

void Terrain::Render(const Shader& shader)
{
	// Activate Texture
	for (unsigned int idx = 0; idx < TERRAIN_LAYER_SIZE; idx++)
	{
		m_TerrainTexture[idx].Activate(GL_TEXTURE0 + idx);
	}

	// Set uniform Texture
	m_TerrainShader.Activate();
	m_TerrainShader.SetInt("u_WaterTexture", 0);
	m_TerrainShader.SetInt("u_SandTexture", 1);
	m_TerrainShader.SetInt("u_GroundTexture", 2);
	m_TerrainShader.SetInt("u_GrassTexture", 3);
	m_TerrainShader.SetInt("u_SnowTexture", 4);

	// Set Terrain Component
	m_TerrainShader.SetFloat("u_Amplitude", m_Amplitude);
	m_TerrainShader.SetInt("u_Octave", m_Octave);
	m_TerrainShader.SetFloat("u_Persistance", m_Persistance);
	m_TerrainShader.SetFloat("u_Lunarcity", m_Lunarcity);
	m_TerrainShader.SetBool("u_IsTerrainSmooth", m_IsSmooth);
	m_TerrainShader.SetInt("u_Seed", m_Seed);
	m_TerrainShader.SetFloat("u_Delta", m_Delta);

	// Set Utilities Component
	m_TerrainShader.SetVec3("u_LightColor", m_LightColor);
	m_TerrainShader.SetBool("u_IsFogActive", m_Fog.IsActive());
	m_TerrainShader.SetFloat("u_MinFogDistance", m_Fog.GetMinDistance());
	m_TerrainShader.SetFloat("u_MaxFogDistance", m_Fog.GetMaxDistance());
	m_TerrainShader.SetFloat("u_Density", m_Fog.GetDensity());
	m_TerrainShader.SetFloat("u_Gradient", m_Fog.GetGradient());
	m_TerrainShader.SetVec3("u_FogColor", m_Fog.GetColor());

	m_TerrainVAO.Bind();
	
	glDrawArrays(GL_PATCHES, 0, NUM_PATCH_PTS * res * res);
}

const Shader& Terrain::GetShader() const { return m_TerrainShader; }
float& Terrain::GetAmplitude() { return m_Amplitude; }
int& Terrain::GetOctave() { return m_Octave; }
float& Terrain::GetPersistance() { return m_Persistance; }
float& Terrain::GetLunarcity() { return m_Lunarcity; }
bool& Terrain::IsSmooth() { return m_IsSmooth; }
int& Terrain::GetSeed() { return m_Seed; }
float& Terrain::GetDelta() { return m_Delta; }
glm::vec3& Terrain::GetLightColor() { return m_LightColor; }
FogFactor& Terrain::GetFog() { return m_Fog; }

void Terrain::InitShader()
{
	m_TerrainShader.AttachShader(BaseShader("Shader/terrain.vert"));
	m_TerrainShader.AttachShader(BaseShader("Shader/terrain.frag"));
	m_TerrainShader.AttachShader(BaseShader("Shader/terrain.tcs"));
	m_TerrainShader.AttachShader(BaseShader("Shader/terrain.tes"));
	m_TerrainShader.LinkProgram();
}
void Terrain::InitTexture()
{
	m_TerrainTexture[TERRAIN_LAYER::WATER].SetTexture("Texture/water.jpg");
	m_TerrainTexture[TERRAIN_LAYER::SAND].SetTexture("Texture/sand.png");
	m_TerrainTexture[TERRAIN_LAYER::GROUND].SetTexture("Texture/ground.jpg");
	m_TerrainTexture[TERRAIN_LAYER::GRASS].SetTexture("Texture/grass.png");
	m_TerrainTexture[TERRAIN_LAYER::SNOW].SetTexture("Texture/snow.png");
}