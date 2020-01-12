//
// Created by syt0r on 10/14/19.
//

#ifndef LAB4_TEXTRENDERER_H
#define LAB4_TEXTRENDERER_H

#include <vector>
#include <string>
#include "Shader.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Renderable.h"

struct Character {
    GLuint     TextureID;  // ID handle of the glyph texture
    glm::ivec2 Size;       // Size of glyph
    glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
    GLuint     Advance;    // Offset to advance to next glyph
};

class TextRenderer : public Renderable {

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
    void render() override;
};


class TextRendererBuilder {
private:
    std::string fontName = "arial.ttf";
    std::string text = "";
    int windowX = 0, windowY = 0, posX = 0, posY = 0, fontSize = 14;
public:

    TextRendererBuilder(int x, int y) {
        windowX = x;
        windowY = y;
    }

    TextRendererBuilder& setFont(const std::string& fontName) {
        this->fontName = fontName;
        return *this;
    }

    TextRendererBuilder& setFontSize(int fontSize) {
        this->fontSize = fontSize;
        return *this;
    }

    TextRendererBuilder& setPosition(int x, int y) {
        this->posX = x;
        this->posY = y;
        return *this;
    }

    TextRendererBuilder& setText(std::string text) {
        this->text = text;
        return *this;
    }

    TextRenderer* build() {
        auto renderer = new TextRenderer(fontName, fontSize, windowX, windowY);
        renderer->setText(text);
        renderer->setTextPosition(posX, posY);
        return renderer;
    }

};


#endif //LAB4_TEXTRENDERER_H
