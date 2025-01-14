/* date = September 16th 2021 3:53 pm */

#ifndef SHADER_H
#define SHADER_H
#include <shadervalues.h>

GLuint compile_shader(GLenum type, const GLchar *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    GLint param;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &param);
    if (!param) {
        GLchar log[4096];
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        fprintf(stderr, "error: %s: %s\n",
                type == GL_FRAGMENT_SHADER ? "frag" : "vert", (char *) log);
        exit(EXIT_FAILURE);
    }
    return shader;
}

GLuint link_program(GLuint vert, GLuint frag)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    GLint param;
    glGetProgramiv(program, GL_LINK_STATUS, &param);
    if (!param) {
        GLchar log[4096];
        glGetProgramInfoLog(program, sizeof(log), NULL, log);
        fprintf(stderr, "error: link: %s\n", (char *) log);
        exit(EXIT_FAILURE);
    }
    return program;
}

void InitShaderWithCharArray()
{
    GLuint vert = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    GLuint frag = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);
    GLuint program = link_program(vert, frag);
    glDeleteShader(vert);
    glDeleteShader(frag);
    
    glUseProgram(program);
}

#endif //SHADER_H
