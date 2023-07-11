#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h> //with glad we get all the opengl headers

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

class Shader {
public:
	//shader program ID
	unsigned int ID;
	
	//construction of shader
	Shader(const char* vertexPath, const char* fragmentPath) {
		//1. retrieve vertex and fragment source from path
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		//ensure that ifstreams objects can throw exceptions
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		//now try to open the files
		try {
			//open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);	

			std::stringstream vShaderStream, fShaderStream;

			//read file buffers into streams				
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			//close file handlers
			vShaderFile.close();
			fShaderFile.close();

			//convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_UNSUCCESFULLY_READ" << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//2. next we compile the shaders

		unsigned int vertex, fragment;
		int success;
		char infolog[512];

		//vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "SHADER");

		//fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "SHADER");

		//shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		//remember to delete shaders
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	//use/activate shader
	void use() {
		glUseProgram(ID);
	}

	//utility uniform functions
	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
	}

private:
	void checkCompileErrors(unsigned int shader, std::string type) {
		int success;
		char infolog[1024];
		if (type != "PROGRAM") {
			//check if there is errors while compiling
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infolog);
				std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infolog << std::endl;
			}
		}
		else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, infolog);
				std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infolog << std::endl;
			}
		}
	}
};

#endif	