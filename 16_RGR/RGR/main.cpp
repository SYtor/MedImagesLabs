#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <glm/glm.hpp>
#include "Drawable.h"
#include "Point.h"
#include "Shader.h"
#include "Slice.h"
#include "CoordinateSystem.h"
#include "Cube.h"

//Screen size
int windowWidth = 600;
int windowHeight = 600;

int rows = 512, columns = 512;

Coordinate pixelSpacing = {0.703125, 0.703125};
float sliceThickness = 5.0;
float slicesSpacing = 1.25;
Coordinate pixelCoords = {367, 298};

Coordinate imagePositionSlice25 = {-157.400, -180.0, -126.750};
float imageOrientationSlice25[] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0};

Coordinate imagePositionSlice1 = {-157.400, -180.0, 28.250};
float imageOrientationSlice1[] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0};

int numOfImageInSeries = 79;

Shader *shader;
std::vector<Drawable *> drawables;

void initDrawableObjects();
void render();
void keyboardInput(unsigned char key, int x, int y);
void updateViewPoint(glm::vec3 cameraPosition);

int main(int argc, char *argv[]) {

    glutInit(&argc, argv);
    glutInitContextVersion(4, 0);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("RGR");
    glewInit();

    initDrawableObjects();

    glutDisplayFunc(render);
    glutKeyboardFunc(keyboardInput);

    glutMainLoop();

    return 0;

}

void initDrawableObjects() {

    Coordinate pixelPhysicalLocation{
            imagePositionSlice25.x + pixelCoords.y * imageOrientationSlice25[0] * pixelSpacing.y + pixelCoords.x * imageOrientationSlice25[3] * pixelSpacing.x,
            imagePositionSlice25.y + pixelCoords.y * imageOrientationSlice25[1] * pixelSpacing.y + pixelCoords.x * imageOrientationSlice25[4] * pixelSpacing.x,
            imagePositionSlice25.z
    };
    printf("Pixel location(x,y,z) = (%.2f,%.2f,%.2f) ", pixelPhysicalLocation.x, pixelPhysicalLocation.y, pixelPhysicalLocation.z);

    float scanBottomZ = imagePositionSlice1.z - numOfImageInSeries * slicesSpacing; //Bottom because slice 1 is higher than slice 25

    Coordinate slice25EndPoint{
            imagePositionSlice25.x + rows * imageOrientationSlice25[0] * pixelSpacing.y + columns * imageOrientationSlice25[3] * pixelSpacing.x,
            imagePositionSlice25.y + rows * imageOrientationSlice25[1] * pixelSpacing.y + columns * imageOrientationSlice25[4] * pixelSpacing.x,
            imagePositionSlice25.z
    };

    float scanZoneHeight = abs(scanBottomZ - imagePositionSlice1.z) + sliceThickness;
    float scanZoneWidth = abs(slice25EndPoint.x - imagePositionSlice25.x);
    float scanZoneDepth = abs(slice25EndPoint.y - imagePositionSlice25.y);

    Coordinate scanZoneCenter{
            (slice25EndPoint.x + imagePositionSlice25.x) / 2,
            (slice25EndPoint.y + imagePositionSlice25.y) / 2,
            (scanBottomZ + imagePositionSlice1.z) / 2
    };

    shader = new Shader();

    glm::mat4 projection = glm::perspective(glm::radians(40.0f), (float) windowWidth / (float) windowHeight, 0.1f,4000.0f);
    glm::mat4 model = glm::mat4(1.0f);

    shader->setMatrix4("projection", projection);
    shader->setMatrix4("model", model);

    updateViewPoint(glm::vec3(1200));

    drawables.push_back(new CoordinateSystem(shader, 500));
    drawables.push_back(new Cube(shader, scanZoneHeight, scanZoneWidth, scanZoneDepth, scanZoneCenter));
    drawables.push_back(
            new Slice(
                    shader,
                    {imagePositionSlice25.x, imagePositionSlice25.y, imagePositionSlice25.z},
                    {slice25EndPoint.x, imagePositionSlice25.y, imagePositionSlice25.z},
                    {slice25EndPoint.x, slice25EndPoint.y, imagePositionSlice25.z},
                    {imagePositionSlice25.x, slice25EndPoint.y, imagePositionSlice25.z}
            )
    );
    drawables.push_back(new Point(shader, pixelPhysicalLocation, 10));

}

void render() {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto d : drawables)
        d->draw();

    glutSwapBuffers();
}

void updateViewPoint(glm::vec3 cameraPosition) {
    glm::vec3 cameraTarget(0,0,0);
    glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);
    glm::vec3 up(0.0f, 0.0f, 1.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    glm::mat4 view = glm::lookAt(
            cameraPosition,
            cameraTarget,
            cameraUp
    );

    shader->setMatrix4("view", view);
}


void keyboardInput(unsigned char key, int x, int y) {
    switch (key) {
        case 'x':
            updateViewPoint(glm::vec3(1500, 0,0));
            break;
        case 'y':
            updateViewPoint(glm::vec3(0, 1500,0));
            break;
        case 'z':
            updateViewPoint(glm::vec3(600, 0,600));
            break;
        case 'd':
            updateViewPoint(glm::vec3(1200));
            break;
    }
    glutPostRedisplay();
}

