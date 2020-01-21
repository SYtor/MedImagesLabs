#include <GL/glew.h>
#include <GL/freeglut.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <iostream>
#include "Image.h"
#include "TextRenderer.h"
#include "DicomFileWrapper.h"

int windowWidth = 520;
int windowHeight = 400;
int imageWidth;
int imageHeight;

DicomFileWrapper* dicomFileWrapper;

std::vector<Renderable*> renderableObjects;

const float givenRescaleSlope = 0.0101769;
const float givenRescaleIntercept = 3.142;
const int upperPixelBound = 250;
const int lowerPixelBound = 85;

struct DisplayMode {
    std::string helpMessage;
    std::string imageType;
    std::string pixelRange;
    std::string rescaleSlope;
    std::string rescaleIntercept;
    const unsigned char* pixels;
    Image* image;
};

std::vector<DisplayMode> modes;
int currentDisplayMode = 0;
std::pair<float, float> rescaleMinMax;

void initDrawableObjects(const std::string &imagePath);
const unsigned char* applyRescale(float rescaleSlope, float rescaleIntercept, const unsigned char* pixels);
void updateTextLabels(const DisplayMode& displayMode);
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
    glutCreateWindow("Lab4");
    glewInit();

    initDrawableObjects(imagePath);

    glutDisplayFunc(render);
    glutKeyboardFunc(keyboardInput);

    glutMainLoop();

    return 0;
}

void initDrawableObjects(const std::string &imagePath) {

    dicomFileWrapper = new DicomFileWrapper(imagePath);

    imageWidth = dicomFileWrapper->getUShort(DcmTagKey(0x0028, 0x0011));
    imageHeight = dicomFileWrapper->getUShort(DcmTagKey(0x0028, 0x0010));
    auto originalPixelData = dicomFileWrapper->getUCharArray(DcmTagKey(0x7FE0, 0x0010));

    DcmCodeString imageTypeCodeString = dicomFileWrapper->getCodeString(DcmTagKey(0x0008, 0x0008));
    OFString pixelDataCharacteristic;
    imageTypeCodeString.getOFString(pixelDataCharacteristic, 0);
    auto imageType = std::string(pixelDataCharacteristic.c_str());

    auto rescaleIntercept = dicomFileWrapper->getDouble(DcmTagKey(0x0028, 0x1052));
    auto rescaleSlope = dicomFileWrapper->getDouble(DcmTagKey(0x0028, 0x1053));
    auto rescaleInterceptString = std::to_string(rescaleIntercept);
    auto rescaleSlopeString = std::to_string(rescaleSlope);

    originalPixelData = applyRescale(rescaleSlope, rescaleIntercept, originalPixelData);
    auto minMaxPixels = DicomFileWrapper::findMinMaxPixel(imageWidth, imageHeight, originalPixelData);

    auto defaultHelpMessage = "Controls: D - next, A - previous, S - save derived";
    DisplayMode originalDisplayMode;
    originalDisplayMode.helpMessage = defaultHelpMessage;
    originalDisplayMode.imageType = imageType;
    originalDisplayMode.pixelRange = std::to_string(minMaxPixels.first).append("-").append(std::to_string(minMaxPixels.second));
    originalDisplayMode.rescaleSlope = rescaleSlopeString;
    originalDisplayMode.rescaleIntercept = rescaleInterceptString;
    originalDisplayMode.pixels = originalPixelData;
    originalDisplayMode.image = new Image(windowWidth, windowHeight, imageWidth, imageHeight, originalPixelData);
    modes.push_back(originalDisplayMode);

    if (imageType == "ORIGINAL") {

        auto updatedRangePixelData = new unsigned char[imageWidth * imageHeight];
        for (int i = 0; i < imageWidth * imageHeight; i++) {
            auto color = *(originalPixelData + i);
            if (color > upperPixelBound) color = upperPixelBound;
            if (color < lowerPixelBound) color = lowerPixelBound;
            *(updatedRangePixelData + i) = color;
        }
        auto updatedRangeMinMax = DicomFileWrapper::findMinMaxPixel(imageWidth, imageHeight, updatedRangePixelData);

        DisplayMode updatedRangeDisplayMode;
        updatedRangeDisplayMode.helpMessage = defaultHelpMessage;
        updatedRangeDisplayMode.imageType = "Temp. Applied pixel range";
        updatedRangeDisplayMode.pixelRange = std::to_string(updatedRangeMinMax.first).append("-").append(std::to_string(updatedRangeMinMax.second));
        updatedRangeDisplayMode.rescaleSlope = originalDisplayMode.rescaleSlope;
        updatedRangeDisplayMode.rescaleIntercept = originalDisplayMode.rescaleIntercept;
        updatedRangeDisplayMode.pixels = updatedRangePixelData;
        updatedRangeDisplayMode.image = new Image(windowWidth, windowHeight, imageWidth, imageHeight, updatedRangePixelData);
        modes.push_back(updatedRangeDisplayMode);

        auto appliedRescalePixelData = applyRescale(givenRescaleSlope, givenRescaleIntercept, updatedRangePixelData);
        auto appliedRescaleMinMax = DicomFileWrapper::findMinMaxPixel(imageWidth, imageHeight, appliedRescalePixelData);

        DisplayMode appliedRescaleDisplayMode;
        appliedRescaleDisplayMode.helpMessage = defaultHelpMessage;
        appliedRescaleDisplayMode.imageType = "Temp. Applied rescale";
        appliedRescaleDisplayMode.pixelRange = std::to_string(appliedRescaleMinMax.first).append("-").append(std::to_string(appliedRescaleMinMax.second));
        appliedRescaleDisplayMode.rescaleSlope = std::to_string(givenRescaleSlope);
        appliedRescaleDisplayMode.rescaleIntercept = std::to_string(givenRescaleIntercept);
        appliedRescaleDisplayMode.pixels = appliedRescalePixelData;
        appliedRescaleDisplayMode.image = new Image(windowWidth, windowHeight, imageWidth, imageHeight, appliedRescalePixelData);
        modes.push_back(appliedRescaleDisplayMode);

    } else {
        modes.at(0).helpMessage = "Displaying derived image";
    }

    updateTextLabels(modes.at(currentDisplayMode));

}

const unsigned char* applyRescale(float rescaleSlope, float rescaleIntercept, const unsigned char* pixels) {
    auto appliedRescalePixelData = new unsigned char[imageWidth * imageHeight];
    auto tempFloatArray = new float[imageWidth * imageHeight];
    float min = 10000.0f, max = 0.0f;
    for (int i = 0; i < imageWidth * imageHeight; i++) {
        float color = *(pixels + i);
        color = rescaleSlope * color + rescaleIntercept;
        *(tempFloatArray + i) = color;
        if (color < min) min = color;
        if (color > max) max = color;
    }
    rescaleMinMax = std::pair(min, max);
    for (int i = 0; i < imageWidth * imageHeight; i++) {
        auto color = *(tempFloatArray + i);
        color = 255.0f * (color - min) / (max - min);
        *(appliedRescalePixelData + i) = (unsigned char) color;
    }
    return appliedRescalePixelData;
}

void updateTextLabels(const DisplayMode& displayMode) {
    renderableObjects.clear();
    auto builder = TextRendererBuilder(windowWidth, windowHeight);
    renderableObjects.push_back(builder.setPosition(0, windowHeight - 16).setText(displayMode.helpMessage).build());
    renderableObjects.push_back(builder.setPosition(0, windowHeight - 32).setText(std::string("Image Type: ").append(displayMode.imageType)).build());
    renderableObjects.push_back(builder.setPosition(0, windowHeight - 48).setText("Pixel Type: UNSIGNED_BYTE").build());
    renderableObjects.push_back(builder.setPosition(0, windowHeight - 64).setText(std::string("Pixel Range(min-max): ").append(displayMode.pixelRange)).build());
    renderableObjects.push_back(builder.setPosition(0, windowHeight - 96).setText(std::string("Rescale Intercept: ").append(displayMode.rescaleIntercept)).build());
    renderableObjects.push_back(builder.setPosition(0, windowHeight - 112).setText(std::string("Rescale Slope: ").append(displayMode.rescaleSlope)).build());
    if (modes.size() == 1 || currentDisplayMode == 2)
        renderableObjects.push_back(
                builder.setPosition(0, windowHeight - 138).setText(
                        std::string("Rescale Pixel Range(min-max): ")
                        .append(std::to_string(rescaleMinMax.first))
                        .append("-")
                        .append(std::to_string(rescaleMinMax.second)
                        )).build()
        );
}


void render() {

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    auto displayMode = modes.at(currentDisplayMode);
    displayMode.image->render();

    for (auto renderable : renderableObjects)
        renderable->render();

    glutSwapBuffers();

}

void keyboardInput(unsigned char key, int x, int y) {
    if (modes.size() == 1)
        return;

    switch (key) {
        case 'd':
            currentDisplayMode = (currentDisplayMode + 1) % (int) modes.size();
            updateTextLabels(modes.at(currentDisplayMode));
            break;
        case 'a': {
            int tempDisplayModeIndex = currentDisplayMode - 1;
            currentDisplayMode = tempDisplayModeIndex < 0 ? (int) modes.size() - 1 : tempDisplayModeIndex;
            updateTextLabels(modes.at(currentDisplayMode));
            break;
        }
        case 's':
            dicomFileWrapper->saveTransformedImage(imageWidth, imageHeight, modes.at(1).pixels, givenRescaleSlope, givenRescaleIntercept);
            std::cout << "Transformed image was saved to file: derived.dcm" << std::endl;
            break;
    }

    glutPostRedisplay();
}

