#include "Image.h"

Image::Image(int windowWidth, int windowHeight, int imageWidth, int imageHeight, const unsigned char *pixelData) {

    float vertexes[] = {
            //Vertices      //Texture Coords
            0, 0,           0, 1,
            1, 0,           1, 1,
            1, 1,           1, 0,
            0, 1,           0, 0
    };
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
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
            "uniform mat4 projection;"
            "uniform mat4 model;"
            "uniform mat4 view;"
            "in vec2 position; "
            "in vec2 texture_coord_in; "
            "out vec2 texture_coord_out; "
            "void main()"
            "{"
            "    gl_Position = projection * view * model * vec4(position, 0.0, 1.0);"
            "    texture_coord_out = texture_coord_in;"
            "}";

    auto fragmentShader =
            "#version 400\n"
            "uniform sampler2D texture1;"
            "in vec2 texture_coord_out;"
            "out vec4 FragColor;"
            "void main()"
            "{"
            "    vec4 tmpColor = texture(texture1, texture_coord_out);"
            "    FragColor = vec4(tmpColor.r, tmpColor.r, tmpColor.r, 1);"
            "}";

    shader = new Shader(vertexShader, fragmentShader);
    glUseProgram(shader->getReference());

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glm::mat4 projection = glm::ortho<float>(0, windowWidth, 0, windowHeight, -1, 1);
    glm::mat4 view = glm::lookAt(
            glm::vec3(0, 0, 1.0f),
            glm::vec3(0, 0, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
    );
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(imageWidth, imageHeight, 1.0f));

    shader->setMatrix4("projection", projection);
    shader->setMatrix4("view", view);
    shader->setMatrix4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imageWidth, imageHeight, 0, GL_RED, GL_UNSIGNED_BYTE, pixelData);

    glUseProgram(shader->getReference());
    glUniform1i(glGetUniformLocation(shader->getReference(), "texture1"), 0);

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