#pragma once

#include "position.h"
#include <string>

class GUI
{
private:
	void initialize(int argc, char** argv, const std::string title, const Position& ptExtent);
	static Position ptExtent;
	static bool initialized;
	static bool isDownPress;
	static bool isUpPress;
	static bool isRightPress;
	static bool isLeftPress;
	static bool isSpacePress;
public:
	static void* p; // void pointer to hold game object
	static void (*callback) (void*); // function pointer
	// Constructors
	GUI() {}
	GUI(int argc, char** argv, const std::string title, const Position& ptExtent)
	{
		GUI::ptExtent = ptExtent;
		initialize(argc, argv, title, ptExtent);
	}
	// Start the OpenGL game loop
	static void run(void (*callBack) (void*), void* p);
	// When a key is pressed
	static void keyEvent(int key);
	// reset the keys that have been pressed
	static void resetKeys();
	// get the extent of the game and/or window
	static Position getExtent() { return ptExtent; }
	// get key press states
	static bool isDown()  { return isDownPress; }
	static bool isUp()    { return isUpPress; }
	static bool isLeft()  { return isLeftPress; }
	static bool isRight() { return isRightPress; }
	static bool isSpace() { return isSpacePress; }
};

// client functions for OpenGL callbacks
void drawCallback();
void motionCallback(int x, int y);
void specialCallback(int key, int x, int y);
void keyboardCallback(unsigned char key, int x, int y);
void mouseCallback(int button, int state, int x, int y);
void reshapeCallback(int width, int height);

// Draw fancy demo
void drawDemo();

// Set the zoom
void zoom(Position zoom);
// Change the current drawing color.
void changeColorRGB(double r, double g, double b);
// 2d rectangles
void drawRectangle2d(const Position& pt1, const Position& pt2, 
	const Position& pt3, const Position& pt4, double depth = 0.0);
void drawSquare2d(const Position& pt, const Position& size, double depth = 0.0);
// 3d rectangles
//void drawRectangle3d(Position pt1, Position pt2, Position pt3, Position pt4);
//void drawSquare3d(Position pt, Position size);

// Draw ship
void drawSpaceship(const Position& center, double rotation = 0.0, double scale = 2.0);