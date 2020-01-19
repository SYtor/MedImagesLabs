#include "Shader.h"
#include <iostream>

Shader::Shader() {

    auto vertexShaderScript =
            "#version 400\n"
            "uniform mat4 projection;"
            "uniform mat4 model;"
            "uniform mat4 view;"
            "layout (location = 0) in vec3 position;"
            "layout (location = 1) in vec4 color_in;"
            "out vec4 fragColor;"
            "void main()"
            "{"
            "    gl_Position = projection * view * model * vec4(position, 1.0);"
            "    fragColor = color_in;"
            "}";

    auto fragmentShaderScript =
            "#version 400\n"
            "in vec4 fragColor;"
            "out vec4 FragColor;"
            "void main()"
            "{"
            "    FragColor = vec4(fragColor);"
            "}";

    GLuint vertexShader = compileShader(vertexShaderScript, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderScript, GL_FRAGMENT_SHADER);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
}

GLuint Shader::compileShader(const char *shaderText, GLenum shaderType) {

    //Load and Compile Shader

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderText, nullptr);
    glCompileShader(shader);

    //Check compile status

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, nullptr, buffer);
        throw std::runtime_error(buffer);
    }

    return shader;
}

void Shader::setMatrix4(const std::string &name, glm::mat4 matrix) {
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setVec3(const std::string &name, glm::vec3 vector) {
    glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &vector[0]);
}