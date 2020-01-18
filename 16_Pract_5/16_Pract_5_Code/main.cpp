#include <GL/glew.h>
#include <GL/freeglut.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <string>
#include <vector>
#include "Image.h"
#include "TextRenderer.h"
#include "Renderable.h"
#include "DicomFileWrapper.h"
#include <math.h>

int windowWidth;
int windowHeight;

int imageWidth;
int imageHeight;

std::vector<Renderable*> renderableObjects;
TextRenderer* cursorPositionText;
TextRenderer* scanPositionText;

OFVector<double> imagePosition;
OFVector<double> imageOrientation;

void render();
double getAngleDegree(double cosValue);
void cursorInput(int x, int y);
void displayCursorPosition(int x, int y);
void displayScanPosition(int x, int y);

int main(int argc, char* argv[]) {

    // Read image

    DicomFileWrapper dicomFileWrapper("DICOM_Image.dcm");

    imageWidth = dicomFileWrapper.getUShort(DcmTagKey(0x0028, 0x0011));
    imageHeight = dicomFileWrapper.getUShort(DcmTagKey(0x0028, 0x0010));
    auto pixelData = dicomFileWrapper.getUCharArray(DcmTagKey(0x7FE0, 0x0010));

    windowWidth = imageWidth * 2;
    windowHeight = imageHeight * 2;

    imagePosition = dicomFileWrapper.getDoubleArray(DcmTagKey(0x0020, 0x0032), 3);
    imageOrientation = dicomFileWrapper.getDoubleArray(DcmTagKey(0x0020, 0x0037), 6);

    // Create window

    glutInit(&argc, argv);
    glutInitContextVersion(4,0);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Lab5");
    glewInit();

    // Init rendering data

    auto image = new Image(windowWidth, windowHeight, imageWidth, imageHeight, pixelData);
    image->setPosition(3.0f * imageWidth / 4.0f, imageHeight / 4.0f);
    image->setRotation(180); //According to 16 variant

    if (getAngleDegree(imageOrientation.at(0)) > 45 && getAngleDegree(imageOrientation.at(imageOrientation.at(4))) > 45)
        image->setRotation(0);

    renderableObjects.push_back(image);

    cursorPositionText = new TextRenderer("arial.ttf", 14, windowWidth, windowHeight);
    cursorPositionText->setTextPosition(0, windowHeight - 16);
    cursorPositionText->setTextColor(0,0,0);
    renderableObjects.push_back(cursorPositionText);

    scanPositionText = new TextRenderer("arial.ttf", 14, windowWidth, windowHeight);
    scanPositionText->setTextPosition(0, windowHeight - 30);
    cursorPositionText->setTextColor(0,0,0);
    renderableObjects.push_back(scanPositionText);

    auto lLabel = new TextRenderer("arial.ttf", 40, windowWidth, windowHeight);
    lLabel->setTextPosition(windowWidth - 20, imageHeight / 2);
    lLabel->setText("L");
    lLabel->setTextColor(1, 1, 1);
    renderableObjects.push_back(lLabel);

    auto aLabel = new TextRenderer("arial.ttf", 40, windowWidth, windowHeight);
    aLabel->setTextPosition(imageWidth * 3 / 2 - 10, 0);
    aLabel->setText("A");
    aLabel->setTextColor(1, 1, 1);
    renderableObjects.push_back(aLabel);

    // Register callback functions and start rendering loop

    glutDisplayFunc(render);
    glutPassiveMotionFunc(cursorInput);

    glutMainLoop();

    return 0;
}

void render() {

    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);

    for(auto renderable : renderableObjects)
        renderable->render();

    glutSwapBuffers();

}

double getAngleDegree(double cosValue) {
    return (int) acos(cosValue) % 360;
}

void cursorInput(int x, int y) {
    displayCursorPosition(x, y);
    displayScanPosition(x, y);
    glutPostRedisplay();
}

void displayCursorPosition(int x, int y) {
    auto text = std::string("Cursor Position: x= ")
            .append(std::to_string(x - imageWidth))
            .append(" y= ")
            .append(std::to_string(imageHeight - y));
    cursorPositionText->setText(text);
}

void displayScanPosition(int x, int y) {
    x = x - imageWidth;
    y = y - imageHeight;
    if (x > 0 && y > 0) {
        auto resultX = imagePosition.at(0) + y * imageOrientation.at(0) + x * imageOrientation.at(3);
        auto resultY = imagePosition.at(1) + y * imageOrientation.at(1) + x * imageOrientation.at(4);
        auto resultZ = imagePosition.at(2) + y * imageOrientation.at(2) + x * imageOrientation.at(5);
        auto text = std::string("Scan Position: x= ")
                .append(std::to_string(resultX))
                .append(" y= ")
                .append(std::to_string(resultY))
                .append(" z= ")
                .append(std::to_string(resultZ));
        scanPositionText->setText(text);
    } else {
        scanPositionText->setText(std::string("Scan Position: Out of bounds"));
    }
}


