#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <glm\glm.hpp>

typedef unsigned int GLuint;

std::string get_file_contents(const char* filename);

class Shader {
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);

    void SetInt(const std::string& name, const int& value);
    void SetIntArray(const std::string& name, int* values, uint32_t count);
    void SetFloat(const std::string& name, const float value);
    void SetFloat2(const std::string& name, const glm::vec2& value);
    void SetFloat3(const std::string& name, const glm::vec3& value);
    void SetFloat4(const std::string& name, const glm::vec4& value);
    void SetMat4(const std::string& name, const glm::mat4& matrix);

	void Activate();
	void Delete();
private:
	void compileErrors(unsigned int shader, const char* type);
};

