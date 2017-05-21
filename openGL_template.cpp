#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "Shader.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define MAX_VERTICES 1024
void key_callback(GLFWwindow*, int, int, int, int);
void mouse_callback(GLFWwindow*, double, double);
void scroll_callback(GLFWwindow*, double, double);
void key_interaction();
void d2xy(int, int*, int*);
void rot(int, int*, int*, int, int);
void draw_points(int);
GLFWwindow* window;
bool start = false;
bool keys[1024];
int main(){
    //GLFW initialization
    glfwInit();
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hilbert's curve", nullptr, nullptr);
    int width, height;
    if(window == nullptr){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0,0,width, height);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    glfwSetKeyCallback(window, key_callback);
    Shader shader("vertex1", "geometry1", "fragment1");
    GLfloat point[] = {
         0.0f, 0.0f, 0.0f
    };
    GLuint VAO, VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);


    GLint levelPowLoc = glGetUniformLocation(shader.Program, "startDistance");
    GLint levelLoc = glGetUniformLocation(shader.Program, "levelFloat");
    GLfloat level = 1;
    GLint verticesDrawn;
    GLint currentVertex;
    GLint currentTime = 100;
    while(!glfwWindowShouldClose(window)){
        //Reset the amount of vertices that need to be drawn each frame.
        verticesDrawn = (level * level) - 1;
        currentVertex = 0;
        if(start){
            start = false;
            currentTime = glfwGetTime();
        }
        //Increase the level each second
        if((int)glfwGetTime() - currentTime == 1){
            if(level < 512){
                level *= 2;
            }
            currentTime = glfwGetTime();
        }

        glfwPollEvents();
        key_interaction();
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.Use();
        glBindVertexArray(VAO);
        glUniform1f(levelLoc, level);
        if(verticesDrawn > MAX_VERTICES){
            //The geometry shader will need to be called multiple times to draw the whole curve.
            currentVertex = 0;
            while(currentVertex <= verticesDrawn){
                glUniform1i(levelPowLoc, currentVertex);
                currentVertex += (MAX_VERTICES - 2);
                glDrawArrays(GL_POINTS, 0, 1);
            }
        }
        else{
            currentVertex = 0;
            glDrawArrays(GL_POINTS, 0, 1);
        }
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(action == GLFW_PRESS){
        keys[key] = true;
    }
    else if (action == GLFW_RELEASE){
        keys[key] = false;
    }
}

void key_interaction()
{
    if(keys[GLFW_KEY_ESCAPE]){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if(keys[GLFW_KEY_X]){
        start = true;
        keys[GLFW_KEY_X] = false;
    }
}

