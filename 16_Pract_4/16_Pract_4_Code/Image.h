#ifndef LAB4_IMAGE_H
#define LAB4_IMAGE_H

#include <GL/glew.h>
#include <string>
#include <vector>
#include "Shader.h"
#include "Renderable.h"

struct DisplayMode {
    std::string title;
    std::string pixelRange;
    GLint texture;
};

class Image : public Renderable {

private:

    Shader *shader;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint texture;
    std::vector<GLuint> textureArray;

public:

    Image(int windowWidth, int windowHeight, int imageWidth, int imageHeight, const unsigned char* pixelData);
    ~Image();
    void setDisplayMode(DisplayMode displayMode);
    void render() override;

};

#endif //LAB4_IMAGE_H
