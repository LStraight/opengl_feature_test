#include "Renderer.hpp"

#include <iostream>

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ")" << std::endl
            << function << ": \n"
            << file << " \n"
            << "line:" << line << std::endl;
        return false;
    }

    return true;
}