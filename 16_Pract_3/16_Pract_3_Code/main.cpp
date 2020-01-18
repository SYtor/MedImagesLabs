#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "Image.h"
#include "TextRenderer.h"
#include "DicomFileWrapper.h"

int windowWidth = 300;
int windowHeight = 300;

Image* image;
TextRenderer* textRenderer;

void render();
void keyboardInput(unsigned char key, int x, int y);

int main(int argc, char* argv[]) {

    glutInit(&argc, argv);
    glutInitContextVersion(4,2);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Lab3");
    glewInit();

    DicomFileWrapper dicomFileWrapper("DICOM_Image_for_Lab_2.dcm");

    int imageWidth = dicomFileWrapper.getUShort(DcmTagKey(0x0028, 0x0011));
    int imageHeight = dicomFileWrapper.getUShort(DcmTagKey(0x0028, 0x0010));
    auto pixelData = dicomFileWrapper.getUCharArray(DcmTagKey(0x7FE0, 0x0010));
    image = new Image(windowWidth, windowHeight, imageWidth, imageHeight, pixelData);

    auto patientWeight = dicomFileWrapper.getString(DcmTagKey(0x0010, 0x1030));
    textRenderer = new TextRenderer("arial.ttf", 14, windowWidth, windowHeight);
    textRenderer->setTextPosition(0, windowHeight - 16);
    textRenderer->setText(std::string("Patient's Weight: ").append(patientWeight));

    glutDisplayFunc(render);
    glutKeyboardFunc(keyboardInput);

    glutMainLoop();

    delete image;
    delete textRenderer;

    return 0;
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