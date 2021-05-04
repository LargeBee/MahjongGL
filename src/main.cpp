#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <shader.hpp>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

//Window Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int, char**) {
    //glfw init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //glfw window creation
    //spdlog::debug("Creating GLFW Window");
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MahjongGL", NULL, NULL);
    if (window == NULL)
    {
        //spdlog::error("Failed to create GLFW Window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

    //glad load opengl function pointers
    //spdlog::debug("Initialising GLAD");
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        //spdlog::error("Failed to initialise GLAD");
        return -1;
    }

    //build and compile shaders
    Shader basicShaders("shader.vs", "shader.fs");

    //create vertex data and vertex attributes
    float vertices[] = {
         0.5f,  0.5f, 0.0f, //top right
         0.5f, -0.5f, 0.0f, //bottom right
        -0.5f, -0.5f, 0.0f, //bottom left
        -0.5f,  0.5f, 0.0f  //top left
    };
    unsigned int indices[] = {
        0, 1, 3,    //first triangle
        1, 2, 3     //second triangle
    };

    //create and bind VBO (Vertex Buffer Object)
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //bind Vertex Array Object, then bind and set vertex buffer and then configure vertex attributes
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //unbind VAO
    glBindVertexArray(0);

    //solid or wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //render loop
    while(!glfwWindowShouldClose(window))
    {
        //input
        processInput(window);

        //render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //draw triangle
        basicShaders.use();

        float timeValue = glfwGetTime();
        float redValue = (sin(timeValue) / 2.0f) + 0.5f;
        float greenValue = (cos(timeValue) / 2.0f) + 0.5f;
        float blueValue = (tan(timeValue) / 2.0f) + 0.5f;
        basicShaders.setVec4("ourColor", redValue, greenValue, blueValue, 1.0f);

        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //glfw swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    //clear all allocated glfw resources
    glfwTerminate();
    return 0;
}

//process input
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//glfw resize the viewport to the window when resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}