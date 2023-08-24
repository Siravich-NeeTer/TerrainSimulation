#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include "Renderer/Shader.h"
#include "Renderer/VertexArrayObject.h"
#include "Renderer/VertexBufferObject.h"

class Water
{
	public:
		Water(int width = 10, int height = 10);
		~Water();

		void Render();

		const Shader& GetShader() const;

	private:
		Shader m_WaterShader;
		VertexArrayObject m_WaterVAO;
		VertexBufferObject m_WaterVBO;

		void InitShader();
};