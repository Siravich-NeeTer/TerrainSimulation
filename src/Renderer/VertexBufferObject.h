#pragma once

#include <glad/glad.h>

class VertexBufferObject
{
	public:
		VertexBufferObject();
		
		void Bind();
		void UnBind();
		
		void Delete();
		void BufferData(size_t dataSize, const void* data, bool dynamic);
		
		GLuint GetID() const;

	private:
		GLuint m_ID;
};