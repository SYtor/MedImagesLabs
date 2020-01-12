//
// Created by syt0r on 10/10/19.
//

#ifndef LAB2_SHADER_H
#define LAB2_SHADER_H

#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {

private:
    GLuint shaderProgram;
    GLuint compileShader(const char *shaderText, GLenum shaderType);
public:
    Shader();
    void setMatrix4(const std::string &name, glm::mat4 matrix);
    GLuint getShader() { return shaderProgram; }

};


#endif //LAB2_SHADER_H
