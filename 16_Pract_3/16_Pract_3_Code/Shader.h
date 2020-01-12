#ifndef LAB3_SHADER_H
#define LAB3_SHADER_H

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {


private:
    GLuint shaderProgram;
    GLuint compileShader(const char *shaderText, GLenum shaderType);

public:
    Shader(std::string vertexShaderScript, std::string fragmentShaderScript);
    void setMatrix4(const std::string &name, glm::mat4 matrix);
    GLuint getReference() { return shaderProgram; }

};


#endif //LAB3_SHADER_H
