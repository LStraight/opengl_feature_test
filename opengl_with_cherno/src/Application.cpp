#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Renderer.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexBufferLayout.hpp"

struct ShaderSources {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderSources ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[int(type)] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int ComplieShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile shader! " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        delete[] message;
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertShader, const std::string& fragShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = ComplieShader(GL_VERTEX_SHADER, vertShader);
    unsigned int fs = ComplieShader(GL_FRAGMENT_SHADER, fragShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    window = glfwCreateWindow(1024, 768, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); 

    if (glewInit() != GLEW_OK) 
        std::cout << "Error" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;

    float position[]{
        -0.5f,  -0.5f,
         0.5f,  -0.5f,
         0.5f,   0.5f,
        -0.5f,   0.5f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int vao;
    GLCALL(glGenVertexArrays(1, &vao));
    GLCALL(glBindVertexArray(vao));

    VertexArray va;
    VertexBuffer vb(position, 8 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    ShaderSources source = ParseShader("res/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    int location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);
    glUniform4f(location, 0.2f, 0.4f, 0.3f, 1.0f);

    GLCALL(glBindVertexArray(0));
    GLCALL(glUseProgram(0));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float increment = 0.005f;
    float incrementg = 0.005f;
    float incrementb = 0.005f;

    while (!glfwWindowShouldClose(window))
    {
        GLCALL(glClear(GL_COLOR_BUFFER_BIT));

        GLCALL(glUseProgram(shader));
        GLCALL(glUniform4f(location, r, g, b, 1.0f));

        //GLCALL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
        //GLCALL(glEnableVertexAttribArray(0));
        //GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
        va.Bind();
        ib.Bind();

        GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));//6是索引的数量
        //if this line has error, the GLCALL was clear the error and get a breakpoint in this line
        //此处GL_INT应改为无符号整型GL_UNSIGNED_INT。嗯，是故意的

        if (r > 1.0f) increment = -0.005f;
        else if (r < 0.0f) increment = 0.005f;
        if (g > 1.0f) incrementg = -0.003f;
        else if (g < 0.0f) incrementg = 0.003f;
        if (b > 1.0f) incrementb = -0.007f;
        else if (b < 0.0f) incrementb = 0.007f;
        r += increment;
        g += incrementg;
        b += incrementb;
        glfwSwapBuffers(window);

        glfwPollEvents();

    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}