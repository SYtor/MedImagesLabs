#include <GL/glew.h>
#include <GL/freeglut.h>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <iostream>
#include <string>
#include <vector>
#include "Image.h"
#include "TextRenderer.h"
#include "Renderable.h"

std::vector<Renderable*> renderableObjects;
TextRenderer* cursorPositionText;
TextRenderer* scanPositionText;

void render();
void cursorInput(int x, int y);
void displayCursorPosition(int x, int y);
void displayScanPosition(int x, int y);

int main(int argc, char* argv[]) {

    // Read image

    auto *dicomImage = new DicomImage("DICOM_Image.dcm");
    int imageWidth = (int) dicomImage->getWidth();
    int imageHeight = (int) dicomImage->getHeight();
    if(!dicomImage->isMonochrome() || dicomImage->getOutputDataSize() / imageHeight / imageWidth != 1)
        throw std::runtime_error("Image not supported");
    auto* pixelData = (unsigned char*) dicomImage->getOutputData(8);

    int windowWidth = imageWidth * 2;
    int windowHeight = imageHeight * 2;

    // Create window

    glutInit(&argc, argv);
    glutInitContextVersion(4,0);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Lab5");
    glewInit();

    // Init rendering data

    auto image = new Image(windowWidth, windowHeight, imageWidth, imageHeight, pixelData);
    image->setPosition((float) imageWidth + 1, 0);
    renderableObjects.push_back(image);

    delete dicomImage;

    cursorPositionText = new TextRenderer("arial.ttf", 14, windowWidth, windowHeight);
    cursorPositionText->setTextPosition(0, windowHeight - 16);
    renderableObjects.push_back(cursorPositionText);

    scanPositionText = new TextRenderer("arial.ttf", 14, windowWidth, windowHeight);
    scanPositionText->setTextPosition(0, windowHeight - 30);
    renderableObjects.push_back(scanPositionText);

    auto l = new TextRenderer("arial.ttf", 40, windowWidth, windowHeight);
    l->setTextPosition(windowWidth - 20, imageHeight / 2);
    l->setText("L");
    l->setTextColor(1,1,1);
    renderableObjects.push_back(l);

    auto a = new TextRenderer("arial.ttf", 40, windowWidth, windowHeight);
    a->setTextPosition(imageWidth * 3 / 2 - 10, 0);
    a->setText("A");
    a->setTextColor(1,1,1);
    renderableObjects.push_back(a);

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

void cursorInput(int x, int y) {
    displayCursorPosition(x, y);
    displayScanPosition(x, y);
    glutPostRedisplay();
}

void displayCursorPosition(int x, int y) {
    auto text = std::string("Cursor Position: ")
            .append(std::to_string(x))
            .append(" ")
            .append(std::to_string(y));
    cursorPositionText->setText(text);
}

void displayScanPosition(int x, int y) {
    auto text = std::string("Scan Position: ")
            .append(std::to_string(x))
            .append(" ")
            .append(std::to_string(y));
    scanPositionText->setText(text);
}


