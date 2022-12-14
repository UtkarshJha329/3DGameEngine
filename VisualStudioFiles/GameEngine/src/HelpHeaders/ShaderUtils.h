#pragma once

#include <string>

#include "../HelpHeaders/Components/Shader.h"

// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
static void checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

//Function to Init shader
static void InitShader(Shader* shader, std::string vertexPath, std::string fragmentPath)
{
	shader->_vertPath = new std::string(vertexPath);
	shader->_fragPath = new std::string(fragmentPath);

	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		std::cout << "\t" << vertexPath << std::endl;
		std::cout << "\t" << fragmentPath << std::endl;

	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	// 2. compile shaders

	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	// shader Program
	shader->ID = glCreateProgram();
	glAttachShader(shader->ID, vertex);
	glAttachShader(shader->ID, fragment);
	glLinkProgram(shader->ID);
	checkCompileErrors(shader->ID, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	shader->type = ShaderType::VertFrag;
}

static void UseShader(Shader* shader)
{
	glUseProgram(shader->ID);
}
// utility uniform functions
// ------------------------------------------------------------------------
static void setUBool(Shader* shader, const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(shader->ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
static void setUInt(Shader* shader, const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(shader->ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
static void setUFloat(Shader* shader, const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(shader->ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
static void setVec3(Shader* shader, const std::string& name, const glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(shader->ID, name.c_str()), 1, &value[0]);
}
// ------------------------------------------------------------------------
static void setVec2(Shader* shader, const std::string& name, const glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(shader->ID, name.c_str()), 1, &value[0]);
}
// ------------------------------------------------------------------------
static void setMat4(Shader* shader, const std::string& name, const glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}