#pragma once
#include <glad\glad.h>


class OpenGLContext
{
};


GLenum glCheckError_(const char* file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)