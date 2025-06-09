#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

typedef unsigned int GLuint;

std::string get_file_contents(const char* filename);

class Shader {
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();
private:
	void compileErrors(unsigned int shader, const char* type);
};

