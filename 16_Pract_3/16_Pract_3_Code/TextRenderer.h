//
// Created by syt0r on 10/14/19.
//

#ifndef LAB3_TEXTRENDERER_H
#define LAB3_TEXTRENDERER_H

#include <vector>
#include <string>
#include "Shader.h"
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    GLuint     TextureID;  // ID handle of the glyph texture
    glm::ivec2 Size;       // Size of glyph
    glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
    GLuint     Advance;    // Offset to advance to next glyph
};

class TextRenderer {

private:

    std::vector<Character> characters;

    Shader* shader;
    GLuint vao;
    GLuint vbo;

    FT_Library ftLibrary;
    FT_Face ftFace;

    bool isVisible = true;

    std::string text;

    int textX = 0;
    int textY = 0;

public:
    TextRenderer(std::string font, int fontSize, int width, int height);
    ~TextRenderer();
    void setText(std::string text);
    void setTextPosition(int x, int y);
    void toggleVisibility();
    void render();
};


#endif //LAB3_TEXTRENDERER_H
