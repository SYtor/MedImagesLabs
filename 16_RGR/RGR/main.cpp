#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <glm/glm.hpp>
#include "Drawable.h"
#include "Point.h"
#include "Shader.h"
#include "Line.h"

//Screen size
float width = 400;
float height = 400;

float pixelSpacing[] {0.976562, 0.976562};
float sliceThickness = 3.75;
float space = 3.75;
float imageOrientation[] {1.0, 0.0, 0.0, 0.0, 1.0, 0.0};
float imagePos1[] {-250.000, -250.000, -120.990};
float imagePos25[] = {-250.000, -250.000, -35.970};
int nrows = 512;
int ncolumns = 512;
int n_slice = 87;

// Slice number
int N = 25;

// Point row, column
int X = 159;
int Y = 134;

Shader* shader;
std::vector<Drawable*> drawables;

void initDrawableObjects();
void render();

int main(int argc, char* argv[]) {

    glutInit(&argc, argv);
    glutInitContextVersion(4,0);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(width, height);
    glutCreateWindow("RGR");
    glewInit();

    initDrawableObjects();

    glutDisplayFunc(render);
    glutIdleFunc(render);

    glutMainLoop();

    return 0;

}

void initDrawableObjects() {

    shader = new Shader();

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 projectionMatrix = glm::ortho(-width, width, -height, height, 1.0f, 100.0f);

    shader->setMatrix4("model", modelMatrix);
    shader->setMatrix4("view", viewMatrix);
    shader->setMatrix4("projection", projectionMatrix);

    //Axes
    drawables.push_back(new Line(shader, {-400, -400}, {400, 400}));
    //drawables.push_back(new Line(shader, {200, 200}, {-200, -200}));

    //Slice

    //Cube

    //Point
    //drawables.push_back(new Point(shader, {0,0}, 5.0f));

}

void render() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    for(Drawable* d : drawables)
        d->draw();

    glutSwapBuffers();
}