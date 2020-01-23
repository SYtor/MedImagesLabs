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

std::vector<Renderable *> renderableObjects;
TextRenderer *cursorPositionText;
TextRenderer *scanPositionText;

OFVector<double> imagePosition;
OFVector<double> imageOrientation;

bool leftIsLeft;
bool topIsAnterior;

void render();

void cursorInput(int x, int y);
void displayCursorPosition(int x, int y);
void displayScanPosition(int x, int y);
void swapX(){}
void swapY(){}


int main(int argc, char *argv[]) {

    // Read image

    DicomFileWrapper dicomFileWrapper("DICOM_Image.dcm");

    imageWidth = dicomFileWrapper.getUShort(DcmTagKey(0x0028, 0x0011));
    imageHeight = dicomFileWrapper.getUShort(DcmTagKey(0x0028, 0x0010));
    auto pixelData = dicomFileWrapper.getUCharArray(DcmTagKey(0x7FE0, 0x0010));

    windowWidth = imageWidth * 2;
    windowHeight = imageHeight * 2;

    imagePosition = dicomFileWrapper.getDoubleArray(DcmTagKey(0x0020, 0x0032), 3);
    imageOrientation = dicomFileWrapper.getDoubleArray(DcmTagKey(0x0020, 0x0037), 6);

    std::string xtext, ytext;
    bool swapX = false, swapY = false;

    if ((abs(imageOrientation.at(0)) + abs(imageOrientation.at(3)) > cos(45 * M_PI / 180)) &&
        (abs(imageOrientation.at(1)) + abs(imageOrientation.at(4)) > cos(45 * M_PI / 180))) // Axial plane
    {
        xtext.append("L");
        if (imageOrientation.at(0) > 0) {
            leftIsLeft = true;
            swapX = true;
            if (imageOrientation.at(0) != 1) {
                if (abs(imageOrientation.at(1)) != 0) {
                    if (imageOrientation.at(1) > 0)
                        xtext.append("F");
                    else if (imageOrientation.at(1) < 0)
                        xtext.append("H");
                }
                if (abs(imageOrientation.at(2)) != 0) {
                    if (imageOrientation.at(2) > 0)
                        xtext.append("P");
                    else if (imageOrientation.at(2) < 0)
                        xtext.append("A");
                }
            }
        } else if (imageOrientation.at(0) < 0) {
            leftIsLeft = false;
            if (imageOrientation.at(0) != -1) {
                if (abs(imageOrientation.at(1)) != 0) {
                    if (imageOrientation.at(1) > 0)
                        xtext.append("H");
                    else if (imageOrientation.at(1) < 0)
                        xtext.append("F");
                }
                if (abs(imageOrientation.at(2)) != 0) {
                    if (imageOrientation.at(2) > 0)
                        xtext.append("A");
                    else if (imageOrientation.at(2) < 0)
                        xtext.append("P");
                }
            }
        }
        ytext.append("A");
        if (imageOrientation.at(4) > 0) {
            topIsAnterior = true;
            swapY = true;
            if (imageOrientation.at(4) != 1) {
                if (imageOrientation.at(3) != 0) {
                    if (imageOrientation.at(3) > 0)
                        ytext.append("R");
                    else if (imageOrientation.at(3) < 0)
                        ytext.append("L");
                }
                if (imageOrientation.at(5) != 0) {
                    if (imageOrientation.at(5) > 0)
                        ytext.append("F");
                    else if (imageOrientation.at(5) < 0)
                        ytext.append("H");
                }
            }
        } else if (imageOrientation.at(4) < 0) {
            topIsAnterior = false;
            if (imageOrientation.at(4) != -1) {
                if (imageOrientation.at(3) != 0) {
                    if (imageOrientation.at(3) > 0)
                        ytext.append("L");
                    else if (imageOrientation.at(3) < 0)
                        ytext.append("R");
                }
                if (imageOrientation.at(5) != 0) {
                    if (imageOrientation.at(5) > 0)
                        ytext.append("H");
                    else if (imageOrientation.at(5) < 0)
                        ytext.append("F");
                }
            }
        }
    }

    auto bitsAllocated = dicomFileWrapper.getUShort(DcmTagKey(0x0028, 0x0100));
    auto rescaleIntercept = dicomFileWrapper.getDouble(DcmTagKey(0x0028, 0x1052));
    auto rescaleSlope = dicomFileWrapper.getDouble(DcmTagKey(0x0028, 0x1053));

    GLenum pixelType;

    if (bitsAllocated == 8 && rescaleIntercept == 0 && rescaleSlope == 1)
        pixelType = GL_UNSIGNED_BYTE;
    else if (bitsAllocated == 32 && rescaleIntercept != 0 && rescaleSlope != 1)
        pixelType = GL_FLOAT;
    else
        throw std::runtime_error("Image not supported");

    glutInit(&argc, argv);
    glutInitContextVersion(4, 0);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Lab5");
    glewInit();

    auto image = new Image(windowWidth, windowHeight, imageWidth, imageHeight,pixelType, pixelData, swapX, swapY);
    image->setPosition(3.0f * imageWidth / 4.0f, imageHeight / 4.0f);

    renderableObjects.push_back(image);

    cursorPositionText = new TextRenderer("arial.ttf", 14, windowWidth, windowHeight);
    cursorPositionText->setTextPosition(0, windowHeight - 16);
    cursorPositionText->setTextColor(0, 0, 0);
    renderableObjects.push_back(cursorPositionText);

    scanPositionText = new TextRenderer("arial.ttf", 14, windowWidth, windowHeight);
    scanPositionText->setTextPosition(0, windowHeight - 30);
    cursorPositionText->setTextColor(0, 0, 0);
    renderableObjects.push_back(scanPositionText);

    auto lLabel = new TextRenderer("arial.ttf", 40, windowWidth, windowHeight);
    lLabel->
            setTextPosition(windowWidth
                            - 20, imageHeight / 2);
    lLabel->setText(xtext);
    lLabel->setTextColor(1, 1, 1);
    renderableObjects.
            push_back(lLabel);

    auto aLabel = new TextRenderer("arial.ttf", 40, windowWidth, windowHeight);
    aLabel->
            setTextPosition(imageWidth
                            * 3 / 2 - 10, 0);
    aLabel->setText(ytext);
    aLabel->setTextColor(1, 1, 1);
    renderableObjects.
            push_back(aLabel);

    glutDisplayFunc(render);
    glutPassiveMotionFunc(cursorInput);

    glutMainLoop();

    return 0;
}

void render() {

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto renderable : renderableObjects)
        renderable->render();

    glutSwapBuffers();

}

void cursorInput(int x, int y) {
    x = x - imageWidth;
    y = imageHeight - y;
    displayCursorPosition(x, y);
    displayScanPosition(x, y);
    glutPostRedisplay();
}

void displayCursorPosition(int x, int y) {
    auto text = std::string("Cursor Position: x= ")
            .append(std::to_string(x))
            .append(" y= ")
            .append(std::to_string(y));
    cursorPositionText->setText(text);
}

void displayScanPosition(int x, int y) {

    y = -y;
    double mm_coords[3];

    if (x > 0 && y > 0){
        int imageX, imageY;
        for (int i = 0; i < 3; i++) {
            if (leftIsLeft)
                imageX = x;
            else
                imageX = imageWidth - x;
            mm_coords[i] = imageOrientation.at(i) * imageX;
            if (topIsAnterior)
                imageY = y;
            else
                imageY = -y + imageHeight;
            mm_coords[i] += imageOrientation.at(i+3) * imageY;
            mm_coords[i] += imagePosition.at(i);
        }

        printf("coords[%i][%i]\n", x, y);
        auto text = std::string("Scan Position: x= ")
                .append(std::to_string(mm_coords[0]))
                .append(" y= ")
                .append(std::to_string(mm_coords[1]))
                .append(" z= ")
                .append(std::to_string(mm_coords[2]));
        scanPositionText->setText(text);

    }
    else
        scanPositionText->setText(std::string("Scan Position: Out of bounds"));
}
