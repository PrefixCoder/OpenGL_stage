#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>



class Shader
{
public:
	GLuint Program;
	Shader(const GLchar * vertexPath, const GLchar * fragmentPath);
	void UseProgram();

	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string &name, float value) const;
	void SetVec3(const std::string &name, float v0, float v1, float v2) const;
	void SetVec3(const std::string &name, glm::vec3 value) const;
	void SetMat3(const std::string &name, glm::mat3 value) const;
	void SetVec4(const std::string &name, float v0, float v1, float v2, float v3) const;
	void SetVec4(const std::string &name, glm::vec4 value) const;
	void SetMat4(const std::string &name, glm::mat4 value) const;
};

#endif

