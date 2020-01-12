#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "CoordinateSystem.h"
#include "Circle.h"

int width = 600;
int height = 600;

Shader* shader;
CoordinateSystem* coordinateSystem;
Circle* circle;

void init(int argc, char* argv[]);
void display(SphericalCoordinates center, int radius);
void render();

int main(int argc, char* argv[]) {

    SphericalCoordinates center{};
    int radius;

    std::cout << "Circle center radial distance = ";
    std::cin >> center.radialDistance;

    std::cout << "Circle center theta = ";
    std::cin >> center.theta;
    center.theta = glm::radians(center.theta);

    std::cout << "Circle center phi = ";
    std::cin >> center.phi;
    center.phi = glm::radians(center.phi);

    std::cout << "Circle radius = ";
    std::cin >> radius;


    init(argc, argv);
    display(center, radius);

    return 0;
}

void init(int argc, char* argv[]) {

    glutInit(&argc, argv);
    glutInitContextVersion(4, 0);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Lab 2. Spherical to Cortasian");

    glewInit();

    glutDisplayFunc(render);

}

void display(SphericalCoordinates center, int radius) {

    shader = new Shader();

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 projectionMatrix = glm::ortho(-width/2.0f, width/2.0f, -height/2.0f, height/2.0f, 1.0f, -1.0f);

    shader->setMatrix4("model", modelMatrix);
    shader->setMatrix4("view", viewMatrix);
    shader->setMatrix4("projection", projectionMatrix);

    coordinateSystem = new CoordinateSystem(shader, width, height);
    circle = new Circle(shader);

    circle->setSphericalParameters(center, radius);

    glutMainLoop();

    delete shader;
    delete coordinateSystem;
    delete circle;

}

void render() {

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    coordinateSystem->render();
    circle->render();

    glutSwapBuffers();

}
