#pragma once 
#include <GL/glew.h>


#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
    x;\
    ASSERT (GLLogCall(#x, __FILE__, __LINE__));
//‘#’可以将变量转化为字符串， __FILE__文件路径, __LINE__是代码行号
//可以在每个opengl的函数上套上CGLCALL，debugger更简单

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);