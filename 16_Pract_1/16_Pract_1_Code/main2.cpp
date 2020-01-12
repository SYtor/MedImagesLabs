#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "Sprite.h"

void render();

Sprite* sprite;

int main(int argc, char* argv[]) {

    glutInit(&argc, argv);
    glutInitContextVersion(4,0);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(400,400);
    glutCreateWindow("Test");

    glutDisplayFunc(render);
    glutIdleFunc(render);

    glewInit();

    // Init
    sprite = new Sprite("textures/dinosaur.png");

    glutMainLoop();

    delete sprite;

    return 0;
}

void render() {

    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);

    sprite->render();

    glutSwapBuffers();

}