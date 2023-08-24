#include "Water.h"

Water::Water(int width, int height)
	: m_WaterShader("WaterShader")
{
	InitShader();

	const float depth = 0.75f;

	std::vector<glm::vec3> m_Vertices;
	
	m_Vertices.push_back({ 0.0f, depth, 0.0f });
	m_Vertices.push_back({ width, depth, 0.0f });
	m_Vertices.push_back({ 0.0f, depth, height });

	m_Vertices.push_back({ width, depth, 0.0f });
	m_Vertices.push_back({ width, depth, height });
	m_Vertices.push_back({ 0.0f, depth, height });

	m_WaterVAO.Bind();

	m_WaterVBO.BufferData(sizeof(glm::vec3) * m_Vertices.size(), m_Vertices.data(), false);
	
	m_WaterVAO.Attribute(m_WaterVBO, 0, 3, GL_FLOAT, sizeof(glm::vec3), 0);

}
Water::~Water() 
{
	m_WaterVAO.Delete();
	m_WaterVBO.Delete();
}

void Water::Render()
{
	m_WaterShader.Activate();

	m_WaterVAO.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

const Shader& Water::GetShader() const { return m_WaterShader; }

void Water::InitShader()
{
	m_WaterShader.AttachShader(BaseShader("Shader/water.vert"));
	m_WaterShader.AttachShader(BaseShader("Shader/water.frag"));
	m_WaterShader.LinkProgram();
}