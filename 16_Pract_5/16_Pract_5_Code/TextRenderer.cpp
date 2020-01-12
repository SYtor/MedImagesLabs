#include "TextRenderer.h"

TextRenderer::TextRenderer(std::string font, int fontSize, int width, int height) {

    FT_Init_FreeType(&ftLibrary);
    FT_New_Face(ftLibrary, font.c_str(), 0,  &ftFace);

    FT_Set_Pixel_Sizes(ftFace, 0, fontSize);

    auto vertexShader =
            "#version 400\n"
            "in vec4 vertex;"
            "out vec2 TexCoords;"
            "uniform mat4 projection;"
            "void main()"
            "{"
            "    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);"
            "    TexCoords = vertex.zw;"
            "}  ";

    auto fragmentShader =
            "#version 400\n"
            "in vec2 TexCoords;"
            "out vec4 color;"
            "uniform sampler2D text;"
            "uniform vec3 textColor;"
            "void main()"
            "{"
            "    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);"
            "    color = vec4(textColor, 1.0) * sampled;"
            "} ";

    shader = new Shader(vertexShader, fragmentShader);

    glUseProgram(shader->getReference());

    glm::mat4 projection = glm::ortho(0.0f, (float) width, 0.0f, (float) height);
    glUniformMatrix4fv(glGetUniformLocation(shader->getReference(), "projection"), 1, GL_FALSE,
            glm::value_ptr(projection));

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte character = 0; character <= 127; character++) {

        FT_Load_Char(ftFace, character, FT_LOAD_RENDER);

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                ftFace->glyph->bitmap.width,
                ftFace->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                ftFace->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character characterObj = {
                texture,
                glm::ivec2(ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows),
                glm::ivec2(ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top),
                (GLuint) ftFace->glyph->advance.x
        };

        characters.push_back(characterObj);

    }

    FT_Done_Face(ftFace);
    FT_Done_FreeType(ftLibrary);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

TextRenderer::~TextRenderer() {
    delete shader;
}

void TextRenderer::setText(std::string text) {
    this->text = text;
}

void TextRenderer::setTextPosition(int x, int y) {
    textX = x;
    textY = y;
}

void TextRenderer::toggleVisibility() {
    isVisible = !isVisible;
}

void TextRenderer::setTextColor(float r, float g, float b) {
    textColor = glm::vec3(r,g,b);
}

void TextRenderer::render() {

    if (isVisible) {

        int x = textX, y = textY, scale = 1;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glUseProgram(shader->getReference());
        shader->setVec3("textColor", textColor);

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(vao);

        for(char character : text) {

            Character ch = characters[character];

            GLfloat xpos = x + ch.Bearing.x * scale;
            GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            GLfloat w = ch.Size.x * scale;
            GLfloat h = ch.Size.y * scale;
            // Update vbo for each character
            GLfloat vertices[6][4] = {
                    { xpos,     ypos + h,   0.0, 0.0 },
                    { xpos,     ypos,       0.0, 1.0 },
                    { xpos + w, ypos,       1.0, 1.0 },

                    { xpos,     ypos + h,   0.0, 0.0 },
                    { xpos + w, ypos,       1.0, 1.0 },
                    { xpos + w, ypos + h,   1.0, 0.0 }
            };
            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // Update content of vbo memory
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)

        }

    }

}
