#include "Image.h"
#include <algorithm>

Image::Image(int windowWidth, int windowHeight, int imageWidth, int imageHeight, const unsigned char *pixelData) {

    numOfPixels = imageWidth * imageHeight;

    // Read pixel data

    for (int y = 0; y < imageHeight; y++) {
        for (int x = 0; x < imageWidth; x++) {

            unsigned char pixel = *(pixelData + imageWidth * y + x);
            float color = (float) pixel / 255.0f;

            defaultData.push_back(x);
            defaultData.push_back(imageHeight - y);
            defaultData.push_back(color); //Red
            defaultData.push_back(color); //Green
            defaultData.push_back(color); //Blue

        }
    }

    // Init OpenGL Data

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    applyScaleType(ScaleType::DEFAULT);

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
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 projectionMatrix = glm::ortho(0.0f, (float) windowWidth, 0.0f, (float) windowHeight, 1.0f, -1.0f);

    shader->setMatrix4("model", modelMatrix);
    shader->setMatrix4("view", viewMatrix);
    shader->setMatrix4("projection", projectionMatrix);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(int)));
    glEnableVertexAttribArray(1);

}

void Image::setRescaleParameters(float rescaleSlope, float rescaleIntercept, int lowerBound, int upperBound) {

    transformedData.clear();

    float lowerBoundScaled = (float) lowerBound / 255.0f;
    float upperBoundScaled = (float) upperBound / 255.0f;

    float min = 255;
    float max = 0;

    for (int i = 0; i < numOfPixels; i++) {

        float x = defaultData[i * 5 + 0];
        float y = defaultData[i * 5 + 1];
        float color = defaultData[i * 5 + 2];

        float modifiedColor = color;
        if (modifiedColor > upperBoundScaled)
            modifiedColor = upperBoundScaled;
        else if (modifiedColor < lowerBoundScaled)
            modifiedColor = lowerBoundScaled;
        else
            modifiedColor = (rescaleSlope * color * 255 + rescaleIntercept);

        if (modifiedColor < min) min = modifiedColor;
        if (modifiedColor > max) max = modifiedColor;

        transformedData.push_back(x);
        transformedData.push_back(y);
        transformedData.push_back(modifiedColor); // Red
        transformedData.push_back(modifiedColor); // Green
        transformedData.push_back(modifiedColor); // Blue

    }

    for (int i = 0; i < numOfPixels; i++) {
        float color = transformedData[i * 5 + 2];
        float normalizedColor = (color - min) / (max - min);
        transformedData.at(i * 5 + 2) = normalizedColor;
        transformedData.at(i * 5 + 3) = normalizedColor;
        transformedData.at(i * 5 + 4) = normalizedColor;
    }

}

void Image::applyScaleType(ScaleType scaleType) {

    std::vector<float> pixelData;

    switch (scaleType) {
        case ScaleType::DEFAULT:
            pixelData = defaultData;
            break;
        case ScaleType::SCALED:
            pixelData = transformedData;
            break;
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, pixelData.size() * sizeof(float), &pixelData[0], GL_DYNAMIC_DRAW);

}

Image::~Image() {
    delete shader;
}

void Image::render() {

    glUseProgram(shader->getReference());
    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, numOfPixels);

}