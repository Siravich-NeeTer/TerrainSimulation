#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include "NoiseGenerator.h"

#include "Renderer/Mesh.h"
#include "Renderer/Texture.h"
#include "Renderer/Shader.h"

const unsigned int NUM_PATCH_PTS = 4;
const int res = 10;

class FogFactor
{
	public:
		bool& IsActive() { return m_IsActive; }
		float& GetMinDistance() { return m_MinDistance; }
		float& GetMaxDistance() { return m_MaxDistance; }
		float& GetDensity() { return m_Density; }
		float& GetGradient() { return m_Gradient; }
		glm::vec3& GetColor() { return m_Color; }

	private:
		bool m_IsActive = true;

		float m_MinDistance = 0.1f;
		float m_MaxDistance = 15.0f;

		float m_Density = 0.1f;
		float m_Gradient = 0.1f;

		glm::vec3 m_Color = glm::vec3(0.9f);
};

class Terrain
{
	public:
		enum TERRAIN_LAYER
		{
			WATER = 0,
			SAND,
			GROUND,
			GRASS,
			SNOW,

			TERRAIN_LAYER_SIZE
		};

		Terrain(int width = 10, int height = 10);
		~Terrain();

		void Render(const Shader& shader);

		const Shader& GetShader() const;
		float& GetAmplitude();
		int& GetOctave();
		float& GetPersistance();
		float& GetLunarcity();
		bool& IsSmooth();
		int& GetSeed();
		float& GetDelta();
		glm::vec3& GetLightColor();
		FogFactor& GetFog();

	private:
		NoiseGenerator m_NoiseGenerator;
		std::vector<float> m_Vertices;
		float m_Amplitude = 2.0f;
		int m_Octave = 8;
		float m_Persistance = 0.5f;
		float m_Lunarcity = 2.0f;
		bool m_IsSmooth = true;
		int m_Seed = 500;
		float m_Delta = 0.005f;
		
		VertexArrayObject m_TerrainVAO;
		VertexBufferObject m_TerrainVBO;

		Texture m_TerrainTexture[TERRAIN_LAYER_SIZE + 1];
		Shader m_TerrainShader;

		glm::vec3 m_LightColor = glm::vec3(1.0f);
		FogFactor m_Fog;

		void InitShader();
		void InitTexture();
};