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
#include "Shader.hpp"
#include "Texture.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"


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
        0.0f, 0.0f, 0.0f, 0.0f,
        500.0f, 0.0f, 1.0f, 0.0f,
        500.0f, 280.0f, 1.0f, 1.0f,
        0.0f, 280.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLCALL(glEnable(GL_BLEND)); //启用blend
    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    //glBlendFunc(src, dest)
    //src = how the src RGBA factor is computed (default is GL_ONE)
    //dest = how the dest RGBA factor is computed (default is FL_ZERO)

    {
        unsigned int vao;
        GLCALL(glGenVertexArrays(1, &vao));
        GLCALL(glBindVertexArray(vao));

        VertexArray va;
        VertexBuffer vb(position, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 0.0f));

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        Texture texture("res/texture/picnic.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);
        //texture.Bind(2);
        //shader.SetUniform1i("u_Texture", 2);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        float increment = 0.005f;
        float incrementg = 0.005f;
        float incrementb = 0.005f;

        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(400, 200, 0);

        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();
            ImGui_ImplGlfwGL3_NewFrame();


            {
                glm::mat4 model = glm::translate(glm::mat4(1.0), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            va.Bind();
            ib.Bind();
            renderer.Draw(va, ib, shader);

            GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            {
                ImGui::SliderFloat3("translation A", &translationA.x, 0.0f, 960.0f);
                ImGui::SliderFloat3("translation B", &translationB.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            glfwPollEvents();

        }
    }//在调用terminaate之前析构变量

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}