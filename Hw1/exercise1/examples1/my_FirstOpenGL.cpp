// A simple introductory program; its main window contains a static picture
// of a triangle, whose three vertices are red, green and blue.  The program
// illustrates viewing with default viewing parameters only.

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>

void init (void)
{
glClearColor (1.0, 1.0, 1.0, 0.0); // select clearing (background) color (white backgroud)
glColor3f(0, 0, 0); // initialize viewing values (black color)
glPointSize(4); // set point size
glMatrixMode(GL_PROJECTION); 
glLoadIdentity(); 
gluOrtho2D(-320.0,320.0,-240.0,240.0); //define a 2-D orthographic projection matrix 
}

void display()
{
glClear(GL_COLOR_BUFFER_BIT); //The buffers currently 
//enabled for color writing
//glBegin (GL_POINTS); 
//glBegin (GL_LINES); 
//glBegin (GL_LINE_LOOP); 
//glBegin (GL_POLYGON); 
glBegin (GL_TRIANGLES); 
glVertex2i(100,50);
glVertex2i(100,150);
glVertex2i(150,150);
glColor3f(1, 0, 0);
glVertex2i(110,60);
glVertex2i(110,160);
glVertex2i(160,160);
glEnd();
glFlush(); //force execution of GL commands
/* 
glPointSize(2);
glBegin (GL_POINTS); 
//glBegin (GL_LINES); 
//glBegin (GL_LINE_LOOP); 
//glBegin (GL_POLYGON); 
//glBegin (GL_TRIANGLES); 
glVertex2i(110,60);
glVertex2i(110,160);
glVertex2i(160,160);
glEnd();
glFlush(); //force execution of GL commands
/* */
}

void keyEvent(unsigned char key,int x,int y){ // triggered when
// a keyboard key is pressed
// x,y mouse location when key is pressed
switch (key) {
case 'Q':
case 'q':
 exit(0);
break; 
default: 
break;
}
}


// Initializes GLUT, the display mode, and main window; registers callbacks;
// enters the main event loop.
int main(int argc, char** argv) {

  glutInit(&argc, argv); //initialize toolkit
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); //set display mode 
  glutInitWindowSize (640, 480); // (50, 50); //set window size
  glutInitWindowPosition (10, 10); //set window initial position
  glutCreateWindow ("My First OpenGL Program"); //creates a window 

  init (); //additional initializatons

  glutDisplayFunc(display); //registers a redraw function, this
//function is called whenever the system determines that 
//the window should be redrawn. This first
//happen when the window is first opened. 

  glutKeyboardFunc(keyEvent); //registers a keyboard event handler

  glutMainLoop(); //perpetual event processing loop
}	
