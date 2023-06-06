#pragma once 
#include <GL/glew.h>


#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
    x;\
    ASSERT (GLLogCall(#x, __FILE__, __LINE__));
//��#�����Խ�����ת��Ϊ�ַ����� __FILE__�ļ�·��, __LINE__�Ǵ����к�
//������ÿ��opengl�ĺ���������CGLCALL��debugger����

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);