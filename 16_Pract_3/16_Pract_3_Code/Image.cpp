#include "Image.h"

Image::Image(int windowWidth, int windowHeight, int imageWidth, int imageHeight, const unsigned char *pixelData) {

    // Read pixel data

    for (int i = 0; i < imageHeight; i++) {
        for (int j = 0; j < imageWidth; j++) {
            unsigned char pixel = *(pixelData + imageWidth * i + j);
            data.push_back(j);
            data.push_back(i);
            data.push_back((float) pixel / 255.0f); //Red
            data.push_back((float) pixel / 255.0f); //Green
            data.push_back((float) pixel / 255.0f); //Blue
        }
    }

    // Init OpenGL Data

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

    auto vertexShader =
            "#version 400\n"
            "in vec2 position; "
            "in vec3 inColor; "
            "out vec3 vertexColor; "
            "uniform mat4 model;"
            "uniform mat4 view;"
            "uniform mat4 projection;"
            "void main()"
            "{"
            "    gl_Position = projection * view * model * vec4(position, 0.0, 1.0);"
            "    vertexColor = inColor;"
            "}";

    auto fragmentShader =
            "#version 400\n"
            "in vec3 vertexColor;"
            "out vec4 FragColor;"
            "void main()"
            "{"
            "    FragColor = vec4(vertexColor, 1.0);"
            "}";

    shader = new Shader(vertexShader, fragmentShader);

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 projectionMatrix = glm::ortho(0.0f, (float) windowWidth,0.0f,(float) windowHeight, 1.0f, -1.0f);

    shader->setMatrix4("model", modelMatrix);
    shader->setMatrix4("view", viewMatrix);
    shader->setMatrix4("projection", projectionMatrix);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(int)));
    glEnableVertexAttribArray(1);

}

Image::~Image() {
    delete shader;
}

void Image::render() {

    glUseProgram(shader->getReference());
    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, data.size() / 5);

}