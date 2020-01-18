#ifndef LAB5_IMAGE_H
#define LAB5_IMAGE_H

#include <GL/glew.h>
#include <string>
#include <vector>
#include "Shader.h"
#include "Renderable.h"

class Image : public Renderable {

private:

    glm::vec3 scale, translation;
    float rotation = 0;

    Shader *shader;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint texture;

    void updateModelMatrix();

public:
    Image(int windowWidth, int windowHeight, int imageWidth, int imageHeight, const unsigned char *pixelData);
    ~Image();

    void setPosition(float x, float y);
    void setRotation(float degrees);
    void render() override;

};

#endif //LAB5_IMAGE_H
