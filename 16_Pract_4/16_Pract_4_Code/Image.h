#ifndef LAB4_IMAGE_H
#define LAB4_IMAGE_H

#include <GL/glew.h>
#include <string>
#include <vector>
#include "Shader.h"
#include "Renderable.h"

enum ScaleType { DEFAULT, SCALED };

class Image : public Renderable {

private:
    Shader *shader;
    GLuint vao;
    GLuint vbo;
    int numOfPixels;
    std::vector<float> defaultData;
    std::vector<float> transformedData;

public:
    Image(int windowWidth, int windowHeight, int imageWidth, int imageHeight, const unsigned char *pixelData);
    ~Image();

    void setRescaleParameters(float rescaleSlope, float rescaleIntercept, int lowerBound, int upperBound);
    void applyScaleType(ScaleType scaleType);
    void render() override;

};

#endif //LAB4_IMAGE_H
