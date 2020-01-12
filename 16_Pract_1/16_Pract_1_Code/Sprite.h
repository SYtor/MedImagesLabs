//
// Created by syt0r on 9/26/19.
//

#ifndef LAB1_SPRITE_H
#define LAB1_SPRITE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sprite {
private:

    glm::mat4 transform;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint transformUniformLocation;

    Shader* shader;
    Texture* texture;

public:
    Sprite(const char* textureFilePath);
    ~Sprite();
    void render();

};


#endif //LAB1_SPRITE_H
