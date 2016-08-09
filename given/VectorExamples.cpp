#include <math.h>
#include "vector.h"
#include <assert.h>
#include "glut.h"

// Constrol constants
const int TimerInterval = 20;
const float CanvasSize = 100;


//<<<<<<<<<<<<<<<<<<<<<<<< glutHandlers >>>>>>>>>>>>>>>>>

// Reshape handler
void reshape(GLsizei w, GLsizei h)
{
  float worldUnitsPerPixel;
  if(w < h)                           // Fit the circle into the rectangle... 
    worldUnitsPerPixel = 1.1*CanvasSize/(w/2);
  else
    worldUnitsPerPixel = 1.1*CanvasSize/(h/2);

  glViewport(0, 0, w, h);            // Set viewport to whole window

  glMatrixMode(GL_PROJECTION);       // Define world boundaries
  glLoadIdentity();                  
  gluOrtho2D(-w/2*worldUnitsPerPixel, 
	      w/2*worldUnitsPerPixel,
	     -h/2*worldUnitsPerPixel, 
	      h/2*worldUnitsPerPixel);
}

// A C function to draw a circle
void drawCircle(vec3_t ctr, float r)
{
  const int numSides = 30;
  glBegin(GL_POLYGON);
  for(int i=0; i<numSides; i++)
  {
    float th=i*(2*3.1415)/(float)numSides;
    glVertex2f(ctr.X() + r*cos(th), ctr.Y() + r*sin(th));
  }
  glEnd();
}

void drawVec(vec3_t& v, vec3_t start)
{
  vec3_t end = start + v;

  glBegin(GL_POINTS);
    glVertex3f(start.X(), start.Y(), start.Z());
  glEnd();

  glBegin(GL_LINES);
    glVertex3f(start.X(), start.Y(), start.Z());
    glVertex3f(end.X(), end.Y(), end.Z());
  glEnd();

}

void drawPoint(vec3_t& p)
{
  glBegin(GL_POINTS);
    glVertex3f(p.X(), p.Y(), p.Z());
  glEnd();
}

vec3_t getPerp(vec3_t& a)
{
  // Cross a with the z axis to get perpendicular in the plane
  vec3_t normal; normal.Cross(vec3_t(0, 0, 1), a);
  normal.Normalize();
  return normal;
}

// Timer Function
static float t = 0.0;
void recomputeFrame(int value)
{
  static float deltaT = .01;
  // Increment t and test for turnaround
  t += deltaT;
  if( t > 1.0) 
  {
    t = 1.0;
    deltaT = -deltaT;
  }
  else if( t < 0)
  {
    t = 0.0;
    deltaT = -deltaT;
  }  

  glutPostRedisplay();               // Ensure we redisplay
  glutTimerFunc(TimerInterval,       // Reregister the timer function
		        recomputeFrame, 1);
}

// Display handler
void renderFrameA(void)
{
  glClear(GL_COLOR_BUFFER_BIT);      // Clear the screen 

  // Draw two vectors a and b
  double theta = 2*3.14*t;
  vec3_t origin(0.0, 0.0, 0.0);
  vec3_t a(100, 0.0, 0.0);
  vec3_t b(80*cos(theta), 80*sin(theta), 0.0);
  glColor3f(1.0, 0.0, 0.0); 
  drawVec(a, origin);
  glColor3f(0.0, 1.0, 0.0); 
  drawVec(b, origin);

  // Compute vector c as projection of b onto a
  glColor3f(0.0, 0.0, 1.0);
  vec3_t c = a;
  c.Normalize();
  c = c * c.Dot(b);
  drawVec(c, origin);

  // Compute vector d as distance from point (origin + b) and line (origin, origin+a)
  vec3_t d;
  d = c-b;
  glColor3f(0.0, 0.0, 1.0);
  drawVec(d, b);
  double distFrombToA = d.Length();

  glutSwapBuffers();                 // Swap the frame into view
}
void renderFrameB(void)
{
  glClear(GL_COLOR_BUFFER_BIT);      // Clear the screen 

  // Find the centroid of a triangle
  // Draw the triangle in red
  double theta = 2*3.14*t;
  vec3_t p1(-60-40*t, 0, 0);
  vec3_t p2(20, 60+45*(1-t), 0);
  vec3_t p3(60, -40-60*t, 0);
  glColor3f(1.0, 0.0, 0.0);
  drawVec(p2-p1, p1);
  drawVec(p3-p2, p2);
  drawVec(p1-p3, p3);

  // Draw the three medians in green
  vec3_t mp1 = (p1 + p2)/2.0;
  vec3_t mp2 = (p2 + p3)/2.0;
  vec3_t mp3 = (p3 + p1)/2.0;
  glColor3f(0.0, 1.0, 0.0);
  drawVec(mp1-p3, p3);
  drawVec(mp2-p1, p1);
  drawVec(mp3-p2, p2);

  // Show that computed centroid shows up at intersection of medians
  vec3_t pc = (p1 + p2 + p3) / 3.0;
  glColor3f(0.0, 0.0, 1.0);
  drawPoint(pc);
 
  glutSwapBuffers();                 // Swap the frame into view
}

void renderFrameC(void)
{
  glClear(GL_COLOR_BUFFER_BIT);      // Clear the screen 
  glColor3f(1.0, 0.0, 0.0);

  // Draw a, b and c that moves between a and b
  vec3_t origin(0, 0, 0);
  vec3_t a(-90, 80, 0);
  vec3_t b(90, 30, 0);
  vec3_t c = a*(1-t) + b*t;
  glColor3f(1.0, 0.0, 0.0);
  drawVec(a, origin);
  glColor3f(1.0, 0.0, 0.0);
  drawVec(b, origin);
  glColor3f(0.0, 1.0, 0.0);
  drawVec(a-b, b);

  glColor3f(0.0, 0.0, 1.0);
  drawVec(c, origin);

  glutSwapBuffers();                 // Swap the frame into view
}
void renderFrameD(void)
{
  glClear(GL_COLOR_BUFFER_BIT);      // Clear the screen 

  // Draw a box of width 20 around the line between a and b
  double theta = 2*3.14*t;
  glColor3f(1.0, 0.0, 0.0);
  vec3_t a(70.0*cos(theta),      70.0*sin(theta),      0.0);
  vec3_t b(60.0*cos(theta+3.14), 60.0*sin(theta+3.14), 0.0);
  glColor3f(1.0, 0.0, 0.0);
  drawVec(b-a, a);

  // Get w as the perpedicular to a-b in the x,y plane.
  vec3_t w = getPerp(a-b);
  w = w*10;

  // Draw the box as a series of vectors
  glColor3f(0.0, 1.0, 0.0);

  vec3_t tmp1 = a+w;
  vec3_t tmp2 = a-w;
  drawVec(tmp1 - tmp2, a-w);

  tmp1 = b+w;
  tmp2 = a+w;
  drawVec(tmp1 - tmp2 , a+w);

  tmp1 = b-w;
  tmp2 = b+w;
  drawVec(tmp1 - tmp2, b+w);

  tmp1 = a-w;
  tmp2 = b-w;
  drawVec(tmp1 - tmp2, b-w);

  glutSwapBuffers();                 // Swap the frame into view
}
void renderFrameE(void)
{
  glClear(GL_COLOR_BUFFER_BIT);      // Clear the screen 
  glColor3f(1.0, 0.0, 0.0);

  glClear(GL_COLOR_BUFFER_BIT);      // Clear the screen 

  // Find the circle through the points of a triangle
  // Draw the triangle in red
  double theta = 2*3.14*t;
  vec3_t p1(-60-40*t, 0, 0);
  vec3_t p2(20, 60+45*(1-t), 0);
  vec3_t p3(60, -40-60*t, 0);
  glColor3f(1.0, 0.0, 0.0);
  drawVec(p2-p1, p1);
  drawVec(p3-p2, p2);
  drawVec(p1-p3, p3);

  // Draw the bisectors in green
  glColor3f(0.0, 1.0, 0.0);
  vec3_t mp1 = (p1 + p2)/2.0;
  vec3_t biD1 = getPerp(p1-p2);
  drawVec(biD1*80, mp1);

  vec3_t mp2 = (p2 + p3)/2.0;
  vec3_t biD2 = getPerp(p2-p3);
  drawVec(biD2*80, mp2);

  vec3_t mp3 = (p3 + p1)/2.0;
  vec3_t biD3 = getPerp(p3-p1);
  drawVec(biD3*80, mp3);

  // Compute the center of the circle ... draw it in blue
  vec3_t ctr = p1 + ((p2-p1) + getPerp(p2-p1)*((p3-p2).Dot(p1-p3)/getPerp(p2-p1).Dot(p1-p3)))*.5;
  glColor3f(0.0, 0.0, 1.0);
  drawPoint(ctr);

  // Compute radius of the circle and draw it in blue
  double r = (p1-ctr).Length();
  drawCircle(ctr, r);
			
  glutSwapBuffers();                 // Swap the frame into view
}

void renderFrameF(void)
{
  glClear(GL_COLOR_BUFFER_BIT);      // Clear the screen 

  // Find the closest distance between two segments.
  // Draw the segments in red
  vec3_t p1(-90+100*t, 30+(1-t)*30, 0);
  vec3_t p2(20, 60+45*(1-t), 0);
  vec3_t p3(60, -80+120*t, 0);
  vec3_t p4(-60+30*(1-t), -80+160*t, 0);
  glColor3f(1.0, 0.0, 0.0);
  drawVec(p2-p1, p1);
  drawVec(p4-p3, p3);

  // Define the lines: L1: p1 + a*u
  //                   L2: p3 + b*v
  //   Intersect when L1=L2 or p1 + a*u = p3 + b*v
  //                  implies     a*u = (p3-p1) + b*v
  //            dot with bperp      u = (p3-p1).Dot(bperp)/bperp.Dot(a) 
  //            dot with aperp     -(p3-p1).Dot(aperp)/b.Dot(aperp) = v
  //  Only if lines are not parallel
  vec3_t a = p2 - p1;
  vec3_t b = p4 - p3;
  if(abs(1-a.Dot(b)) > .0001)
  {
    vec3_t c = p3 - p1;
    double u = c.Dot(getPerp(b))/getPerp(b).Dot(a); 
    double v = -c.Dot(getPerp(a))/b.Dot(getPerp(a));
    vec3_t ep1 = p1 + a*u;
    vec3_t ep2 = p3 + b*v;
    if( u >= 0 && u <= 1 && v >= 0 && v <= 1)
    { 
	glColor3f(0.0, 1.0, 0.0);
	drawVec(ep2-ep1, ep1);
    }
  }
  glutSwapBuffers();                 // Swap the frame into view
}

void renderFrameG(void)
{
  // Algorithm transcribed from...
  // http://softsurfer.com/Archive/algorithm_0106/algorithm_0106.htm#dist3D_Segment_to_Segment() 
  glClear(GL_COLOR_BUFFER_BIT);      // Clear the screen 

  vec3_t s1p1(-90+100*t, 30+(1-t)*30, 0);
  vec3_t s1p2(20, 60-100*(1-t), 0);
  vec3_t s2p1(60, -80+120*t, 0);
  vec3_t s2p2(-60+30*(1-t), -80+160*t, 0);
  glColor3f(1.0, 0.0, 0.0);
  drawVec(s1p2-s1p1, s1p1);
  drawVec(s2p2-s2p1, s2p1);

  vec3_t u = s1p2 - s1p1;
  vec3_t v = s2p2 - s2p1;
  vec3_t w = s1p1 - s2p1;

  double    a = u.Dot(u);        // always >= 0
  double    b = u.Dot(v);
  double    c = v.Dot(v);        // always >= 0
  double    d = u.Dot(w);
  double    e = v.Dot(w);
  double    D = a*c - b*b;       // always >= 0
  double    sc, sN, sD = D;      // sc = sN / sD, default sD = D >= 0
  double    tc, tN, tD = D;      // tc = tN / tD, default tD = D >= 0

  // compute the line parameters of the two closest points
  if (D < .0001) 
  { 
    // the lines are almost parallel
    sN = 0.0;        // force using point P0 on segment S1
    sD = 1.0;        // to prevent possible division by 0.0 later
    tN = e;
    tD = c;
  }
  else 
  {
    // get the closest points on the infinite lines
    sN = (b*e - c*d);
    tN = (a*e - b*d);
    if (sN < 0.0) 
    {       // sc < 0 => the s=0 edge is visible
      sN = 0.0;
      tN = e;
      tD = c;
    }
    else if (sN > sD) 
    {  
      // sc > 1 => the s=1 edge is visible
      sN = sD;
      tN = e + b;
      tD = c;
    }
  }

  if (tN < 0.0) 
  { 
    // tc < 0 => the t=0 edge is visible
    tN = 0.0;
    // recompute sc for this edge
    if (-d < 0.0)
      sN = 0.0;
    else if (-d > a)
      sN = sD;
    else 
    {
      sN = -d;
      sD = a;
    }
  }
  else if (tN > tD) 
  { 
     // tc > 1 => the t=1 edge is visible
    tN = tD;

    // recompute sc for this edge
    if ((-d + b) < 0.0)
      sN = 0;
    else if ((-d + b) > a)
      sN = sD;
    else 
    {
      sN = (-d + b);
      sD = a;
    }
  }
  
  // finally do the division to get sc and tc
  sc = (abs(sN) < .00001 ? 0.0 : sN / sD);
  tc = (abs(tN) < .00001 ? 0.0 : tN / tD);

  // get the two closest points
  glColor3f(0.0, 1.0, 0.0);
  vec3_t p1 = s1p1 + u*sc;
  vec3_t p2 = s2p1 + v*tc;
  drawVec(p1 - p2, p2);

  glutSwapBuffers();                 // Swap the frame into view
}


void renderFrameH(void)
{
  glClear(GL_COLOR_BUFFER_BIT);      // Clear the screen 
  glColor3f(1.0, 0.0, 0.0);

  // Intersect ray and line...
  // Draw the ray p1 + a*u  and line (point p3 and normal n) in red
  vec3_t p1(0-100*t, 0+(1-t)*100, 0);
  vec3_t p2(20+50*t, 60-75*(1-t), 0);
  vec3_t a = p2-p1;

  vec3_t p3(60, -80+120*t, 0);
  vec3_t p4(-60+30*(1-t), -80+160*t, 0);
  vec3_t n;
  n = getPerp(p4-p3);
  glColor3f(1.0, 0.0, 0.0);
  drawVec(a*2, p1);
  glColor3f(0.0, 0.0, 1.0);
  drawVec((p4-p3)*5, p3);
  drawVec((p4-p3)*-5, p3);
  drawVec(n*20, p3);

  //   Intersect when L1=L2 ... means L1 point at some t will satisfy L2 equation 
  //                  implies   n.Dot((p1+a*uhit)-p3) = 0
  //                            n.Dot((p1-p3) + a*uhit) = 0
  //                            n.Dot(p1-p3) + n.Dot(a*uhit) = 0
  //                            n.Dot(p1-p3) = -n.Dot(a*uhit)
  //                            n.Dot(p1-p3) = -n.Dot(a)*uhit
  //                   -n.Dot(p1-p3)/n.Dot(a) = uhit 
  if(abs(1-n.Dot(a)) > .0001)
  {
    double uhit = -n.Dot(p1-p3)/n.Dot(a); 
    vec3_t xsec = p1 + a*uhit;
    glColor3f(0.0, 1.0, 0.0);
    drawPoint(xsec);
  }
  glutSwapBuffers();                 // Swap the frame into view
}

void right_menu(int id)
{
   static int on = 0;

   switch(id)
   {
   case 1: //"A"
     glutDisplayFunc(renderFrameA);                  // register redraw function
     break;
   case 2: //"B"
     glutDisplayFunc(renderFrameB);                  // register redraw function
     break;
   case 3: //"C"
     glutDisplayFunc(renderFrameC);                  // register redraw function
     break;
   case 4: //"D"
     glutDisplayFunc(renderFrameD);                  // register redraw function
     break;
   case 5: //"E"
     glutDisplayFunc(renderFrameE);                  // register redraw function
     break;
   case 6: //"F"
     glutDisplayFunc(renderFrameF);                  // register redraw function
     break;
   case 7: //"G"
     glutDisplayFunc(renderFrameG);                  // register redraw function
     break;
   case 8: //"H"
     glutDisplayFunc(renderFrameH);                  // register redraw function
     break;
   } 

}

void defineMenus()
{
  // Make the right mouse menu and attach the
  glutCreateMenu(right_menu);
  glutAddMenuEntry("A",       1);
  glutAddMenuEntry("B",       2);
  glutAddMenuEntry("C",       3);
  glutAddMenuEntry("D",       4);
  glutAddMenuEntry("E",       5);
  glutAddMenuEntry("F",       6);
  glutAddMenuEntry("G",       7);
  glutAddMenuEntry("H",       8);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
void myInit(void)
{
  glClearColor(1.0,1.0,1.0,0.0);       // set white background color
  glLineWidth(4.0);		       // a ‘dot’ is 4 by 4 pixels
  glPointSize(10.0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glClear(GL_COLOR_BUFFER_BIT);        // clear the screen 
}

//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
void main(int argc, char** argv)
{
  glutInit(&argc, argv);                         // initialize the toolkit
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);   // set display mode
  glutInitWindowSize(640,480);                   // set window size
  glutInitWindowPosition(100, 150);              // set window position on screen
  glutCreateWindow("Vector Examples");                // open the screen window
  
  glutDisplayFunc(renderFrameA);                  // register redraw function
  glutTimerFunc(TimerInterval, recomputeFrame,1); // register timer function
  glutReshapeFunc(reshape);                      // register reshape function

  reshape(640, 480);                             // set the window boundaries
  myInit();                                      // color, line width, etc..

  defineMenus();                                 // Put up a menu of choices
                   
  glutMainLoop(); 		                 // go into a perpetual loop
}


