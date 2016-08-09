#ifndef _RENDER_H
#define _RENDER_H

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "glut.h"

class Render
{
public:
	
	//constructor definition
	Render();
	//destructor definition
	~Render();

	// These functions are defined public so they can be called by the main function in main.cpp
	void display(void);
	void init(void);
	void reshape(int w, int h);
	void mouseClick(int button, int state, int x, int y);
	void mouseMove(int x, int y);
	void keyPos(unsigned char key, int x, int y);
	void drawRotatingRectangle(void);
	
private:
	// These functions are private and cannot be called anywhere outside of render class
	void drawTextureBox(void);
	void clamp(float v0, float v1, float v2);

	
};

#endif
