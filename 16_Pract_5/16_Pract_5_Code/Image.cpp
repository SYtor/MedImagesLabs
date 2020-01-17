#include "Image.h"

Image::Image(int windowWidth, int windowHeight, int imageWidth, int imageHeight, const unsigned char *pixelData) {



    float vertexes[] = {
            //Vertices      //Texture Coords
            -0.5, -0.5,     0,0,
            0.5, -0.5,      1,0,
            0.5, 0.5,       1,1,
            -0.5, 0.5,      0,1
    };
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), &vertexes, GL_STATIC_DRAW);

    GLuint elements[] = {
            0, 1, 2, //First Triangle
            2, 3, 0  // Second Triangle
    };

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), &elements, GL_STATIC_DRAW);

    auto vertexShader =
            "#version 400\n"
            "uniform mat4 transform;"
            "in vec2 position; "
            "in vec2 texture_coord_in; "
            "out vec2 texture_coord_out; "
            "void main()"
            "{"
            "    gl_Position = transform * vec4(position, 0.0, 1.0);"
            "    texture_coord_out = texture_coord_in;"
            "}";

    auto fragmentShader =
            "#version 400\n"
            "uniform sampler2D texture1;"
            "in vec2 texture_coord_in;"
            "out vec4 FragColor;"
            "void main()"
            "{"
            "    FragColor = texture(texture1, texture_coord_in);"
            "}";

    shader = new Shader(vertexShader, fragmentShader);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    setPosition(0,0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    auto data = new unsigned char[imageWidth * imageHeight];
    for (int i = 0; i < imageWidth; ++i) {
        for (int j = 0; j < imageHeight; ++j) {
            data[imageWidth * i + imageHeight] = *(pixelData + imageWidth * i + imageHeight);
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, imageWidth, imageHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    GLenum res = glGetError();

    glUseProgram(shader->getReference());
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->getReference(), "texture1"), 0);

    printf("zzzzzzzzzzzzzz %i\n", res);

}

void Image::setPosition(float x, float y) {
    glm::mat4 positionMatrix = glm::mat4(1.0f);
    //positionMatrix = glm::translate(positionMatrix, glm::vec3(x, y, 0.0f));
    shader->setMatrix4("transform", positionMatrix);
}

Image::~Image() {
    delete shader;
}

void Image::render() {

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glUseProgram(shader->getReference());
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

}