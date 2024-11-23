#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <common.h>
#include <shader.h>
#include <particles.h>
#include <shadervalues.h>
#include <types.h>

#define MAX_PARTICLES 17000
#define PARTICLES_PER_FRAME 250

int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(600, 400, "OpenGL Particles", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        return -1;
    }
    
    InitShaderWithCharArray();
    Particle particles[MAX_PARTICLES];
    
    InitParticles(particles, MAX_PARTICLES);
    glEnable(GL_BLEND);
    unsigned int VBO, VAO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(Particle), 0, GL_STREAM_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
    glEnableVertexAttribArray(0);
    
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Deltas
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;
    float gravity = -0.00009f;
    int last_dead_particle = 0;
    
    while (!glfwWindowShouldClose(window))
    {
        ProcessInput(window);
        
        /* calculate delta*/
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        ClearWindow();
        UpdateParticles(particles, sizeof(particles)/sizeof(particles[0]), PARTICLES_PER_FRAME, deltaTime, gravity, &last_dead_particle);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindVertexArray(VAO);
        
        glPointSize(2);
        // NOTE(krypton) : Update the entire struct and send the vbo the entire struct
        // thanks to : http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle) * MAX_PARTICLES, &particles);
        
        glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    
    return 0;
    
}