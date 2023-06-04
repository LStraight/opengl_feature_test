#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) 
        std::cout << "Error" << std::endl;
    //glewInit应该在创建上下文之后调用
    std::cout << glGetString(GL_VERSION) << std::endl;

    float position[6]{
        -0.5f,  -0.5f,
        0.0f,   0.5f,
        0.5f,   -0.5f
    };

    uint32_t buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), position, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    /*
    * 
    * 顶点属性数量：此处只有二维坐标，所以当传入2，如果除了坐标还有别的属性，比如纹理法向，则按实际填入
    * 数据类型：此处为float
    * 需不需要正则化：此处已正则化，填false
    * 步长：每个顶点（类比结构体）属性的大小，此处是顶点属性数量*float大小
    * 
    */

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}