
/* Orientation of Polygons (windings)
	FRONT Face: Vertices in counterclockwise order
	BACK Face: Vertices in clockwise order
	Polygons must preserve orientation
	Finding normal vectors (normals) from polygonal data

	 New OpenGL Commands:
	 void glFrontFace(GLenum mode); 
	By default mode is GL_CCW, which corresponds to a counterclockwise orientation.
	If mode is GL_CW, faces with a clockwise orientation are considered front- facing.
*/

/*
	
	Culling (Diagrafh pisw epifaneiwn)
	New OpenGL Commands:
	glEnable(GL_CULL_FACE); // Ej Orismou den fainontai oi pisw pleyres 
	glCullFace( GL_FRONT ); // Den fainontai oi mprosta pleyres 
*/


#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>

GLint w=640;
GLint h=480;


void myInit(void)
{
	glClearColor(0.8, 0.8, 0.8, 0.0); 
	//glFrontFace(GL_CW); // Front face if vertices are in clockwise order
	glEnable(GL_CULL_FACE); // Ej Orismou den fainontai oi pisw pleyres */
	//glCullFace( GL_FRONT ); // Den fainontai oi mprosta pleyres 
	glCullFace( GL_BACK );
	
	glPointSize(4);							 
	glMatrixMode(GL_PROJECTION); // update projection
	glLoadIdentity();
	//gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h); //coordinates are floating ponts - gluOrtho2D follows OpenGL convention
	gluOrtho2D(-300.0, 400.0, -250.0, 250.0);														
			
}


void myDisplay(void) // (re)display callback
{
	//glViewport(w/2,0, w/2,h/2); // The ratio width/height must the same to avoid destortion
	glClear(GL_COLOR_BUFFER_BIT);    // clear th window

	
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	glVertex2i(0,0);
	glEnd();

	glColor3f(0.2, 1.0, 0.5);

	// we give polygon's vertices in counter-clockwise order.
	glBegin(GL_POLYGON);
		glVertex2f(25.0, 25.0);
		glVertex2f(270.0, 25.0);
		glVertex2f(270.0, 270.0);
		glVertex2f(25.0, 270.0);
	glEnd();
	
	
	glFlush(); 
}



int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
	glutInitWindowSize(w, h);
	glutInitWindowPosition(100,100);
	glutCreateWindow("modelTransformations");
	glutDisplayFunc(myDisplay);
	
	myInit();
	
	glutMainLoop();
	return 0;
}
