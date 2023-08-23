#include "BaseShader.h"

BaseShader::BaseShader(const char* shaderPath)
	: m_ShaderPath(shaderPath)
{
	std::string shaderCode = LoadShaderCodeFromFile(shaderPath);
	const char* shaderString = shaderCode.c_str();

	m_ShaderType = GetShaderType(shaderPath);
	m_ShaderID = glCreateShader(m_ShaderType.type);
	glShaderSource(m_ShaderID, 1, &shaderString, NULL);
	glCompileShader(m_ShaderID);
	CheckCompileError(m_ShaderID, m_ShaderType.name.c_str(), GetShaderName(shaderPath));
}
BaseShader::~BaseShader()
{

}

std::string BaseShader::LoadShaderCodeFromFile(const char* shaderPath)
{
	std::string shaderCode;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;

		// Read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();

		shaderFile.close();
		// Convert stream into string
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "ERROR::SHADER " << GetShaderName(shaderPath) << " FILE_NOT_SUCCESFULLY_READ" << "\n";
	}
	return shaderCode;
}