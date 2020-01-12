#include <GL/glew.h>
#include <GL/freeglut.h>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <iostream>
#include "Image.h"
#include "TextRenderer.h"

int windowWidth = 300;
int windowHeight = 300;

Image* image;
TextRenderer* textRenderer;

void initDrawableObjects();
void render();
void keyboardInput(unsigned char key, int x, int y);

int main(int argc, char* argv[]) {

    glutInit(&argc, argv);
    glutInitContextVersion(4,2);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Lab3");
    glewInit();

    initDrawableObjects();

    glutDisplayFunc(render);
    glutKeyboardFunc(keyboardInput);

    glutMainLoop();

    delete image;
    delete textRenderer;

    return 0;
}

void initDrawableObjects() {

    // Load pixel data and init image

    auto *dicomImage = new DicomImage("DICOM_Image_for_Lab_2.dcm");
    int imageWidth = (int) dicomImage->getWidth();
    int imageHeight = (int) dicomImage->getHeight();
    if(!dicomImage->isMonochrome() || dicomImage->getOutputDataSize() / imageHeight / imageWidth != 1)
        throw std::runtime_error("Image not supported");
    auto* pixelData = (unsigned char*) dicomImage->getOutputData(8);

    image = new Image(windowWidth, windowHeight, imageWidth, imageHeight, pixelData);

    delete dicomImage;

    // Load patient weight and init text renderer

    DcmFileFormat dcmFileFormat;
    dcmFileFormat.loadFile("DICOM_Image_for_Lab_2.dcm");
    OFString patientWeight;
    DcmTagKey weightTagKey = DcmTagKey(0x0010, 0x1030);
    dcmFileFormat.getDataset()->findAndGetOFString(weightTagKey, patientWeight);

    textRenderer = new TextRenderer("arial.ttf", 14, windowWidth, windowHeight);
    textRenderer->setTextPosition(0, windowHeight - 16);
    textRenderer->setText(std::string("Patient's Weight: ").append(patientWeight.c_str()));

}

void render() {

    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);

    image->render();
    textRenderer->render();

    glutSwapBuffers();

}

void keyboardInput(unsigned char key, int x, int y) {
    textRenderer->toggleVisibility();
    glutPostRedisplay();
}