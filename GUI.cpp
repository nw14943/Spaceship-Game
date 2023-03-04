#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <openGL/gl.h>    // Main OpenGL library
#include <GLUT/glut.h>    // Second OpenGL library
#endif // __APPLE__

#ifdef __linux__
#include <GL/gl.h>    // Main OpenGL library
#include <GL/glut.h>  // Second OpenGL library
#endif // __linux__

#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <Gl/glut.h>        // OpenGL library we copied
#include <ctime>            // for ::Sleep();
#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>
#endif // _WIN32

#include <string>
#include <iostream>
#include "position.h"
#include "velocity.h"
#include "GUI.h"

// Sets the static Position variables.
double Position::virtualPixelConversionX = 1.0;
double Position::virtualPixelConversionY = 1.0;
double Position::virtualPixelConversionZ = 1.0;

// Sets the public static GUI variables
void* GUI::p = NULL; // game object pointer
void(*GUI::callback)(void*) = NULL; // function pointer

// Sets the private static GUI variables
Position GUI::ptExtent;
bool GUI::initialized = false;
bool GUI::isDownPress = false;
bool GUI::isUpPress = false;
bool GUI::isRightPress = false;
bool GUI::isLeftPress = false;
bool GUI::isSpacePress = false;


/***********************************************
* GUI : Initialize
* Setup the graphics and OpenGL
***********************************************/
void GUI::initialize(int argc, char** argv, const std::string title, const Position& ptExtent)
{

    // set up the random number generator
    srand((unsigned int)time(NULL));

    // divide by 2 because 0,0 is the center of the screen
    //Position::setPixelConversion(ptExtent.getX() / 2.0, ptExtent.getY() / 2.0);

    // create the window
    glutInit(&argc, NULL);
    glutInitWindowSize(ptExtent.getX(), ptExtent.getY());

    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("game");
    glutIgnoreKeyRepeat(true);

    glClearColor(0, 0, 0, 0); // clear color is black
    glEnable(GL_DEPTH_TEST); // setup the depth

    // if the extent has a Z component that isn't 0
    // could probably just use the zoom function.
    if (ptExtent.getZ())
        glScaled(2.0 / ptExtent.getX(), 2.0 / ptExtent.getY(), 2.0 / ptExtent.getZ());
    else
        glScaled(2.0 / ptExtent.getX(), 2.0 / ptExtent.getY(), 1.0);

    // Setup the GL callback functions
    glutDisplayFunc(drawCallback);
    glutIdleFunc(drawCallback);
    glutMouseFunc(mouseCallback);
    glutKeyboardFunc(keyboardCallback);
    glutKeyboardFunc(keyboardCallback);
    glutKeyboardUpFunc(keyboardCallback);
    glutSpecialFunc(specialCallback);
    glutSpecialUpFunc(specialCallback);
    glutReshapeFunc(reshapeCallback);
}

/***********************************************
* GUI : Run
* Run the main OpenGL loop with the proper
* client callback and game object.
***********************************************/
void GUI::run(void (*callBack) (void*), void* p)
{
    // Set callback and game object.
    GUI::callback = callBack;
    GUI::p = p;

    // Start the game loop.
    glutMainLoop();
}

/***********************************************
* GUI : Key Event
* This is called when a key is pressed or
* released.
***********************************************/
void GUI::keyEvent(int key)
{
    // Toggle a key
    switch (key)
    {
    case GLUT_KEY_DOWN:
        isDownPress = !isDownPress;
        break;
    case GLUT_KEY_UP:
        isUpPress = !isUpPress;
        break;
    case GLUT_KEY_RIGHT:
        isRightPress = !isRightPress;
        break;
    case GLUT_KEY_LEFT:
        isLeftPress = !isLeftPress;
        break;
    case ' ':
        isSpacePress = !isSpacePress;
        break;
    }
}

/***********************************************
* GUI : Reset Keys
* This is called to clear the inputs.
***********************************************/
void GUI::resetKeys()
{
    initialized = false;
    isDownPress = false;
    isUpPress = false;
    isRightPress = false;
    isLeftPress = false;
    isSpacePress = false;
}





/***********************************************
* Draw Callback
* This is set as OpenGL's draw callback function
***********************************************/
void drawCallback() // Note: Objects drawn first will be on top
{
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Call the client's callback function.
    GUI::callback(GUI::p);

    // Finish up with the drawings.
    // Swaps the double Buffers
    glFlush();
    glutSwapBuffers();
}

/***********************************************
* Special Callback
* This is set as OpenGL's special callback
* function. This is used for key Events that 
* are irregular (used for arrow keys).
***********************************************/
void specialCallback(int key, int x, int y)
{
    std::cout << "Special Event" << std::endl;
    GUI::keyEvent(key);
}

/***********************************************
* Keyboard Callback
* This is set as OpenGL's keyboard callback
* function. This is used for key Events that
* are regular (letters, space, etc.).
***********************************************/
void keyboardCallback(unsigned char key, int x, int y)
{
    std::cout << "Keyboard Event" << std::endl;
    GUI::keyEvent(key);
}

/***********************************************
* Mouse Callback
* This is set as OpenGL's mouse callback
* function. This is used for mouse button events
* and also gives the position of the mouse when
* clicked.
***********************************************/
void mouseCallback(int button, int state, int x, int y)
{
    if ((button == GLUT_LEFT_BUTTON) && state == GLUT_DOWN)
    {
        std::cout << "Left mouse" << std::endl;
    }
    if ((button == GLUT_RIGHT_BUTTON) && state == GLUT_DOWN)
    {
        std::cout << "Right mouse" << std::endl;
    }
}

/***********************************************
* Reshape Callback
* This is set as OpenGL's reshape callback
* function. This is used for window resizing.
* It gives the new width and height.
***********************************************/
void reshapeCallback(int width, int height)
{
    // Resize the window back to what it was.
    Position extent = GUI::getExtent();
    glutReshapeWindow(extent.getX(), extent.getY());
}


/***********************************************
* Draw Demo
* This draws a fancy 3d demo. It is not used
* for the spaceship game.
***********************************************/
void drawDemo()
{
    // draw a white square on the x z axis
    glColor3d(1.0, 1.0, 1.0); // RGB 0 Black - 1 White
    glBegin(GL_POLYGON);
    glVertex3d(-0.5, 0.0, -0.5);
    glVertex3d(0.5, 0.0, -0.5);
    glVertex3d(0.5, 0.0, 0.5);
    glVertex3d(-0.5, 0.0, 0.5);
    glEnd();

    // draw a white square on the x y axis
    glBegin(GL_POLYGON);
    glVertex3d(-0.5, -0.5, 0.0);
    glVertex3d(0.5, -0.5, 0.0);
    glVertex3d(0.5, 0.5, 0.0);
    glVertex3d(-0.5, 0.5, 0.0);
    glEnd();

    // draw a white square on the y z axis
    glBegin(GL_POLYGON);
    glVertex3d(0.0, -0.5, -0.5);
    glVertex3d(0.0, 0.5, -0.5);
    glVertex3d(0.0, 0.5, 0.5);
    glVertex3d(0.0, -0.5, 0.5);
    glEnd();

    // draw a white wire cube at 0 0 0
    glColor3d(1.0, 1.0, 1.0); // RGB 0 Black - 1 White
    glutWireCube(GLdouble(0.75));
    // draw a black solid cube at 0 0 0
    glColor3d(0.0, 0.0, 0.0); // RGB 0 Black - 1 White
    glutSolidCube(GLdouble(0.75));
    // Rotate the context x, y, and z every frame.
    glRotated(1.0, 1.0, 1.0, 1.0);
}

/***********************************************
* Zoom
* Sets the zoom for the current context
***********************************************/
void zoom(Position zoom)
{
    // If 0 do no scaling.
    if (zoom.getX() == 0.0)
        zoom.setX(1.0);
    if (zoom.getY() == 0.0)
        zoom.setY(1.0);
    if (zoom.getZ() == 0.0)
        zoom.setZ(1.0);
    // scale the context
    // X, Y, and Z should be from 0-1 (I think?)
    glScaled(zoom.getX(), zoom.getY(), zoom.getZ());
}

/***********************************************
* Change Color RGB
* changes the color based on numbers from 0-255
***********************************************/
void changeColorRGB(double r, double g, double b)
{
    // glColor3d takes numbers from 0-1
    glColor3d((r / 255.0), (g / 255.0), (b / 255.0));
}

/***********************************************
* Draw Rectangle 2d
* draws a rectangle on a single plane
***********************************************/
void drawRectangle2d(const Position& pt1, const Position& pt2, 
    const  Position& pt3, const Position& pt4, double depth)
{
    glBegin(GL_POLYGON);
    glVertex3d(pt1.getpxX(), pt1.getpxY(), depth);
    glVertex3d(pt2.getpxX(), pt2.getpxY(), depth);
    glVertex3d(pt3.getpxX(), pt3.getpxY(), depth);
    glVertex3d(pt4.getpxX(), pt4.getpxY(), depth);
    glEnd();
}

/***********************************************
* Draw Square 2d
* draws a square on a single plane
***********************************************/
void drawSquare2d(const Position& pt, const Position& size, double depth)
{
    glBegin(GL_POLYGON);
    glVertex3d(pt.getpxX()                , pt.getpxY()                , depth);
    glVertex3d(pt.getpxX() + size.getpxX(), pt.getpxY()                , depth);
    glVertex3d(pt.getpxX() + size.getpxX(), pt.getpxY() + size.getpxX(), depth);
    glVertex3d(pt.getpxX()                , pt.getpxY() + size.getpxX(), depth);
    glEnd();
}

/***********************************************
* Draw Spaceship
* draws a simple 2d spaceship.
***********************************************/
void drawSpaceship(const Position& center, double rotation, double scale)
{
    // Draw a white X on the center of the spaceship
    /*changeColorRGB(255.0, 255.0, 255.0);
    glBegin(GL_LINES);
    glVertex3d(center.getpxX() + scale, center.getpxY() + scale, 0.0);
    glVertex3d(center.getpxX() - scale, center.getpxY() - scale, 0.0);

    glVertex3d(center.getpxX() - scale, center.getpxY() + scale, 0.0);
    glVertex3d(center.getpxX() + scale, center.getpxY() - scale, 0.0);
    glEnd();*/

    // Change color to dull grey
    changeColorRGB(128.0, 128.0, 128.0);

    // Draw a rotated rectangle
    glBegin(GL_POLYGON);
    // Position that is not rotated.
    Position pos1(-4.0 * scale, -1.0 * scale);
    // Position if it were rotated.
    Position r1;
    r1 = pos1.offsetRotate(center, rotation);
    glVertex3d(r1.getpxX(), r1.getpxY(), 0.0);
    pos1.addX(8.0 * scale);
    r1 = pos1.offsetRotate(center, rotation);
    glVertex3d(r1.getpxX(), r1.getpxY(), 0.0);
    pos1.addY(4.0 * scale);
    r1 = pos1.offsetRotate(center, rotation);
    glVertex3d(r1.getpxX(), r1.getpxY(), 0.0);
    pos1.addX(-3.0 * scale);
    pos1.addY(2.0 * scale);
    r1 = pos1.offsetRotate(center, rotation);
    glVertex3d(r1.getpxX(), r1.getpxY(), 0.0);
    pos1.addX(-2.0 * scale);
    r1 = pos1.offsetRotate(center, rotation);
    glVertex3d(r1.getpxX(), r1.getpxY(), 0.0);
    pos1.addX(-3.0 * scale);
    pos1.addY(-2.0 * scale);
    r1 = pos1.offsetRotate(center, rotation);
    glVertex3d(r1.getpxX(), r1.getpxY(), 0.0);
    glEnd();


    // Draw a rotated rectangle
    glBegin(GL_POLYGON);
    // Position that is not rotated.
    Position pos2(-3.0 * scale, -5.0 * scale);
    // Position if it were rotated.
    Position r2;
    r2 = pos2.offsetRotate(center, rotation);
    glVertex3d(r2.getpxX(), r2.getpxY(), 0.0);
    pos2.addX(6.0 * scale);
    r2 = pos2.offsetRotate(center, rotation);
    glVertex3d(r2.getpxX(), r2.getpxY(), 0.0);
    pos2.addY(2.0 * scale);
    r2 = pos2.offsetRotate(center, rotation);
    glVertex3d(r2.getpxX(), r2.getpxY(), 0.0);
    pos2.addX(-6.0 * scale);
    r2 = pos2.offsetRotate(center, rotation);
    glVertex3d(r2.getpxX(), r2.getpxY(), 0.0);
    glEnd();


    // Draw a rotated triangle
    glBegin(GL_TRIANGLES);
    // Position that is not rotated.
    Position pos3(-3.0 * scale, -5.0 * scale);
    // Position if it were rotated.
    Position r3;
    r3 = pos3.offsetRotate(center, rotation);
    glVertex3d(r3.getpxX(), r3.getpxY(), 0.0);
    pos3.addX(3.0 * scale);
    pos3.addY(5.0 * scale);
    r3 = pos3.offsetRotate(center, rotation);
    glVertex3d(r3.getpxX(), r3.getpxY(), 0.0);
    pos3.addX(3.0 * scale);
    pos3.addY(-5.0 * scale);
    r3 = pos3.offsetRotate(center, rotation);
    glVertex3d(r3.getpxX(), r3.getpxY(), 0.0);
    glEnd();
}