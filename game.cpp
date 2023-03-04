

#define _USE_MATH_DEFINES
#include <cmath>

#include "GUI.h"
#include "position.h"
#include "velocity.h"
#include "space.h"
#include <iostream>

/***********************************************
* Callback
* This function runs every frame after all of
* the other OpenGL callback functions defined
* elsewhere.
***********************************************/
void callBack(void* p)
{
    // Set up the game object pointer.
    Space* space = (Space*)p;
    
    // Get the time since last executed.
    double dtime = space->getdTime();

    // Fancy 3d Demo. (doesn't work with the scaling)
    //drawDemo();

    // If the up key is pressed.
    if (GUI::isUp())
    {   
        // Add positive velocity in the current direction
        // of the ship.
        space->shipVelocity.addTotal(1.0, space->shipAngle);
        //std::cout << "Up" << std::endl;
    }
    // If the down key is pressed.
    if (GUI::isDown())
    {
        // Add negative velocity in the current direction
        // of the ship.
        space->shipVelocity.addTotal(-0.5, space->shipAngle);
        //std::cout << "Down" << std::endl;
    }
    // If the right key is pressed.
    if (GUI::isRight())
    {
        // Add positive rotation (clockwise) to the ship
        // based on the time since last frame.
        space->shipAngle += (M_PI / 3.0) * dtime;
        //std::cout << "Right" << std::endl;
    }
    // If the left key is pressed.
    if (GUI::isLeft())
    {
        // Add negative rotation (counter-clockwise) to the ship
        // based on the time since last frame.
        space->shipAngle -= (M_PI / 3.0) * dtime;
        //std::cout << "Left" << std::endl;
    }
    // If the space key is pressed.
    if (GUI::isSpace())
    {
        // Stop the ship.
        space->shipVelocity.set(0.0, 0.0);
        /*std::cout << "Space " << space->shipPosition.getX()
            << " " << space->shipPosition.getY() << std::endl;*/
    }

    // Apply the velocity to the ship's position based on the
    // time since last frame.
    space->shipVelocity.apply(space->shipPosition, dtime);
    // Draw the spaceship with a specific rotation.
    drawSpaceship(space->shipPosition,space->shipAngle);


    // Draw dull green debug square.
    /*changeColorRGB(0.0, 128.0, 0.0);
    drawSquare2d(Position(0.0, 0.0), Position(256.0, 256.0));

    // Draw dull red debug square.
    changeColorRGB(128.0, 0.0, 0.0);
    drawSquare2d(Position(0.0, 0.0), Position(-256.0, -256.0));*/

    // Allow the time to be calculated again.
    space->unlockTime();
}


/***********************************************
* Main
* This is the main function. It the first code
* executed in the program.
***********************************************/
#ifdef _win32
#include <windows.h>
int winapi winmain(
    _in_ hinstance hinstance,
    _in_opt_ hinstance hprevinstance,
    _in_ pstr pcmdline,
    _in_ int ncmdshow)
#else // !_win32
int main(int argc, char** argv)
#endif // !_WIN32
{
    // Define the scaling of the game.
    Position extent(512.0, 512.0, 512.0);

    // Initialize the GUI
    GUI gui(argc, argv, "game", extent);

    // Create the game object
    Space space(extent);

    // Start OpenGL with the game object and
    // callback function.
    gui.run(callBack, &space);

}