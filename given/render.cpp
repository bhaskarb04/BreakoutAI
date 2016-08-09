#include "render.h"

#define TIMER 33

using namespace std;

void recomputeFrame(int value);
float myrot;	

// Default constructor
// used for initializing any user defined variables
Render::Render()
{

}

// Default destructor
Render::~Render()
{


}

void Render::init(void)
{
	// clear the background to the color values specified in the parentheses
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
}

void Render::reshape(int w, int h)
{
	glViewport(0,0,(GLsizei) w, (GLsizei) h);
}

void Render::mouseClick(int button, int state, int x, int y)
{
	glutPostRedisplay();
}

void Render::mouseMove(int x, int y)
{
	glutPostRedisplay();
}

void Render::keyPos(unsigned char key, int x, int y)
{
	switch (key)
	{
		default:
			break;

	    case 27:	// this is for the 'Esc' key on the keyboard, quit the program when the esc key is pressed
		case 'q':	// application exits if 'q' key is hit
			exit (0);
	}
	glutPostRedisplay();
}

void recomputeFrame(int value)
{
	myrot += 1.0f;
	if(myrot > 360.0f)
		myrot -= 360.0f;
	glutPostRedisplay();
}


void Render::display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Specifies which matrix stack is the target for subsequent matrix operations
	// In this example, the projection matrix is set to perspective projection matrix stack
	glMatrixMode(GL_PROJECTION);
	// all matrix values from previous frames set to identity
	glLoadIdentity();
	
	// use glOrtho for a 3D orthogonal projection or gluOrtho2D for 2D orthogonal projection
	gluOrtho2D(-20, 20, -20, 20);

	// Applies subsequent matrix operations to the modelview matrix stack.
	glMatrixMode(GL_MODELVIEW);
	// Clears all the previously loaded values in the modelview matrix
	glLoadIdentity();

	// animate the rectangle by rotating on z-axis
	glutTimerFunc(TIMER, recomputeFrame, 0);

	// draw rectangle
	drawRotatingRectangle();

	// this allows opengl to wait for the draw buffer to be ready in the background for the next frame
	// therefore, while the current buffer is being drawn in the current frame, a buffer is set ready to draw on frame+1
	// this call is effective only when GL_DOUBLE is enabled in glutInitDisplayMode in the main function
	// It is recommended to use glutSwapBuffers and double buffering always
	glutSwapBuffers();
}

void Render::drawRotatingRectangle(void)
{
	glRotatef(myrot, 0, 0, 1);
	glBegin(GL_QUADS);
		glColor3f(1.0, 0.8, 0.1);
		glVertex2i(-10, -10);

		glColor3f(0.7, 0.1, 0.1);
		glVertex2i(10, -10);

		glColor3f(0.1, 0.7, 0.1);
		glVertex2i(10, 10);

		glColor3f(0.1, 0.1, 0.7);
		glVertex2i(-10, 10);
	glEnd();
}

// this is for clamping the numbers between 0 & 360. used for rotation values in the mouse move function
void Render::clamp(float v0, float v1, float v2)
{
	if (v0 > 360 || v0 < -360)
		v0 = 0;
	if (v1 > 360 || v1 < -360)
		v1 = 0;
	if (v2 > 360 || v2 < -360)
		v2 = 0;
}

