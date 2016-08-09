#include <math.h>
#include <assert.h>
#include "vector.h"
#include "glut.h"

// Constrol constants
const int TimerInterval = 20;
const float BallSize = 100;
const float CircleSize = 2000;
const float BallVel[2] = {8, 100};
const float StartPercent = .40;

void drawCircle(float r, float ctrx, float ctry);
void reflect(vec3_t n);
void renderFrame(void);
void recomputeFrame(int value);
void reshape(GLsizei w, GLsizei h);
void myInit(void);


// A C function to draw a circle
void drawCircle(float r, float ctrx, float ctry)
{
  const int numSides = 30;
  glBegin(GL_POLYGON);
  for(int i=0; i<numSides; i++)
  {
    float th=i*(2*3.1415)/(float)numSides;
    glVertex2f(ctrx + r*cos(th), ctry + r*sin(th));
  }
  glEnd();
}

// Ball Class
class ball
{
public:
  // Ball constructor
  ball() 
  {
    ctr.Set(CircleSize*.40, 0, 0);
    vel.Set(BallVel[0], BallVel[1], 0);
    r = BallSize;
	numPts = 0;
  }

  // Draw the ball ... and its trail
  void draw()
  {
    glColor3f(0.0, 0.0, 0.0);
    drawCircle(r, ctr.X(), ctr.Y());

    // Draw the trail
    glBegin(GL_LINE_STRIP);
    for(int i=0; i<numPts; i++)
      glVertex2f(points[0][i], points[1][i]);
    glEnd();
  }

  // Move the ball...remember the trail
  //    collide with the circle
  void move()
  {
    ctr = ctr + vel;
    points[0][numPts]   = ctr.X();
    points[1][numPts++] = ctr.Y();

    // Collide with the circle
    collideWithCircle();
  }

  // Detect collision with circle and reflect
  void collideWithCircle()
  {
    vec3_t c(0,0,0);
    vec3_t d = ctr - c;

    if(d.Length() >= (CircleSize-BallSize))
    {
      d.Normalize();
      reflect(d*-1);
    }
  }

  // Compute a new velocity by reflecting from wall with 
  //   normal n
  void reflect(vec3_t n)
  {
    // Want n to be normalized
    assert(abs(1.0 - n.Length()) < .00001);

    // vel = vel-2*n.Dot(vel)*n
    // recompute velocity as a reflection off a wall with given normal. 
	double s = 2*n.Dot(vel);
	vec3_t tmp = n*s;
	vel = vel - tmp;
  }

  // Store the center and velocity as vectors ... store radius as float.
  vec3_t ctr;
  vec3_t vel;
  GLfloat r;

  // Ball trail
  float points[2][100000];
  int numPts;

};

/////////////////////////////////////////
// Global definition of the ball
//////////////////////////////////////// 
ball B;


//<<<<<<<<<<<<<<<<<<<<<<<< glutHandlers >>>>>>>>>>>>>>>>>
// Display handler
void renderFrame(void)
{
  glClear(GL_COLOR_BUFFER_BIT);      // Clear the screen 

  glutTimerFunc(TimerInterval, recomputeFrame,1);// register timer function

  glColor3f(1.0, 0.0, 0.0);
  drawCircle(CircleSize, 0.0, 0.0);  // Draw the center circle

  B.draw();                          // Draw the ball

  glutSwapBuffers();                 // Swap the frame into view
}

// Timer Function
void recomputeFrame(int value)
{
  B.move();                          // Move the ball

  glutPostRedisplay();               // Ensure we redisplay
}

// Reshape handler
void reshape(GLsizei w, GLsizei h)
{
  float worldUnitsPerPixel;
  if(w < h)                           // Fit the circle into the rectangle... 
    worldUnitsPerPixel = 1.1*CircleSize/(w/2);
  else
    worldUnitsPerPixel = 1.1*CircleSize/(h/2);

  glViewport(0, 0, w, h);            // Set viewport to whole window

  glMatrixMode(GL_PROJECTION);       // Define world boundaries
  glLoadIdentity();                  
  gluOrtho2D(-w/2*worldUnitsPerPixel, 
	      w/2*worldUnitsPerPixel,
	     -h/2*worldUnitsPerPixel, 
	      h/2*worldUnitsPerPixel);
}

//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
void myInit(void)
{
  glClearColor(1.0,1.0,1.0,0.0);       // set white background color
  glColor3f(1.0f, 0.0f, 0.0f);         // set the drawing color 
  glLineWidth(4.0);		       // a ‘dot’ is 4 by 4 pixels
  glClear(GL_COLOR_BUFFER_BIT);        // clear the screen 
}

//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char** argv)
{
  glutInit(&argc, argv);                         // initialize the toolkit
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);   // set display mode
  glutInitWindowSize(640,480);                   // set window size
  glutInitWindowPosition(100, 150);              // set window position on screen
  glutCreateWindow("Reflection");                // open the screen window
  
  glutDisplayFunc(renderFrame);                  // register redraw function
  glutReshapeFunc(reshape);                      // register reshape function

  myInit();                                      // color, line width, etc..
                  
  glutMainLoop(); 		                 // go into a perpetual loop
  return 0;
}


