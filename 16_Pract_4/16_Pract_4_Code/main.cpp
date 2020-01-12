#include <GL/glew.h>
#include <GL/freeglut.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <iostream>
#include "Image.h"
#include "TextRenderer.h"
#include "DicomFileWrapper.h"

int windowWidth = 520;
int windowHeight = 400;

DicomFileWrapper* dicomFileWrapper;

Image *image;
std::vector<Renderable *> renderableObjects;

const char *helpMessage = "Controls: A - apply transform; R - Restore original; S - save transformed image; ";

const float rescaleSlope = 0.0101769;
const float rescaleIntercept = 3.142;
const int upperPixelBound = 250;
const int lowerPixelBound = 85;

void initDrawableObjects(const std::string &imagePath);

void render();

void keyboardInput(unsigned char key, int x, int y);

int main(int argc, char *argv[]) {

    std::string imagePath;
    std::cout << "Enter path to image: ";
    std::cin >> imagePath;

    glutInit(&argc, argv);
    glutInitContextVersion(4, 0);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Lab3");
    glewInit();

    initDrawableObjects(imagePath);

    glutDisplayFunc(render);
    glutKeyboardFunc(keyboardInput);

    glutMainLoop();

    return 0;
}

void initDrawableObjects(const std::string &imagePath) {

    dicomFileWrapper = new DicomFileWrapper(imagePath);

    auto imageTypeString = "Image Type: " + dicomFileWrapper->getString(DcmTagKey(0x0008, 0x0008));
    auto rescaleInterceptString =
            "Rescale Intercept: " + std::to_string(dicomFileWrapper->getDouble(DcmTagKey(0x0028, 0x1052)));
    auto rescaleSlopeString = "Rescale Slope: " + std::to_string(dicomFileWrapper->getDouble(DcmTagKey(0x0028, 0x1053)));

    auto minMaxPixels = dicomFileWrapper->findMinMaxPixel();
    auto minPixelString = "Min Pixel: " + std::to_string(minMaxPixels.first);
    auto maxPixelString = "Max Pixel: " + std::to_string(minMaxPixels.second);

    auto *pixelData = (unsigned char *) dicomFileWrapper->getImageOutputData(8);

    image = new Image(windowWidth, windowHeight, dicomFileWrapper->getImageWidth(), dicomFileWrapper->getImageHeight(), pixelData);
    image->setRescaleParameters(rescaleSlope, rescaleIntercept, lowerPixelBound, upperPixelBound);

    // Init text renderer

    auto builder = TextRendererBuilder(windowWidth, windowHeight);

    auto helpMessageRenderer = builder.setPosition(0, windowHeight - 16).setText(helpMessage).build();
    auto imageTypeRenderer = builder.setPosition(0, windowHeight - 32).setText(imageTypeString).build();
    auto pixelTypeRenderer = builder.setPosition(0, windowHeight - 48).setText(helpMessage).build();
    auto minPixelValueRenderer = builder.setPosition(0, windowHeight - 64).setText(minPixelString).build();
    auto maxPixelValueRenderer = builder.setPosition(0, windowHeight - 80).setText(maxPixelString).build();
    auto rescaleInterceptRenderer = builder.setPosition(0, windowHeight - 96).setText(rescaleInterceptString).build();
    auto rescaleSlopeRenderer = builder.setPosition(0, windowHeight - 112).setText(rescaleSlopeString).build();

    renderableObjects.push_back(image);
    renderableObjects.push_back(helpMessageRenderer);
    renderableObjects.push_back(imageTypeRenderer);
    renderableObjects.push_back(pixelTypeRenderer);
    renderableObjects.push_back(minPixelValueRenderer);
    renderableObjects.push_back(maxPixelValueRenderer);
    renderableObjects.push_back(rescaleSlopeRenderer);
    renderableObjects.push_back(rescaleInterceptRenderer);

}

void render() {

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto renderable : renderableObjects)
        renderable->render();

    glutSwapBuffers();

}

void keyboardInput(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
            image->applyScaleType(ScaleType::SCALED);
            break;
        case 'r':
            image->applyScaleType(ScaleType::DEFAULT);
            break;
        case 's':
            dicomFileWrapper->saveTransformedImage();
            std::cout << "Transformed image was saved to file: transformed.dcm" << std::endl;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

