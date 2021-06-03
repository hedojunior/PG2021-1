#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

//STB
#include "stb_image.h"

class Shader
{
public:
	GLuint ID;
	const char* uniformName;
	glm::vec3 uniformVectorValue;
	float uniformSingleValue;
	bool usesVectorValueUniform;

	// Constructor generates the shader on the fly
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		uniformName = "";
		uniformSingleValue = NULL;
		usesVectorValueUniform = false;

		// 1. Retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensures ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try
		{
			// Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();
		// 2. Compile shaders
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];
		// Vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// Print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		// Print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// Shader Program
		this->ID = glCreateProgram();
		glAttachShader(this->ID, vertex);
		glAttachShader(this->ID, fragment);
		glLinkProgram(this->ID);
		// Print linking errors if any
		glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		// Delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}
	// Uses the current shader
	void Use()
	{
		glUseProgram(0);
		glUseProgram(this->ID);
	}

	void attachUniforms()
	{
		if (uniformName && usesVectorValueUniform)
		{
			glUniform3f(glGetUniformLocation(ID, uniformName), uniformVectorValue.x, uniformVectorValue.y, uniformVectorValue.z);
		}
		else if (uniformName && !usesVectorValueUniform && uniformSingleValue)
		{
			glUniform1f(glGetUniformLocation(ID, uniformName), uniformSingleValue);
		}
	}

	void setUniform(const char* name, glm::vec3 values)
	{
		uniformName = name;
		uniformVectorValue = values;
		usesVectorValueUniform = true;
	}

	void setUniform(const char* name, float value)
	{
		uniformName = name;
		uniformSingleValue = value;
	}

};

#endif
