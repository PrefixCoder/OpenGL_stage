#pragma once
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H


#include <GL/glew.h>


class VertexBuffer
{
private:
	GLuint ID;
public:
	VertexBuffer(const void* data, GLuint size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};

#endif

