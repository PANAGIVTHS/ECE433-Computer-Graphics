
// Eisagwgh shmeiwn me to pontiki 
//--------------------------------------------------------------------------------------------------------------------------------
// H entolh glutPostRedisplay()://requests that the callback registered with
								//glutDisplayFunc() be called as soon as possible. This is preferred over
								//calling your rendering routine directly, since the user may have interacted with
								//your application and user input events need to be processed.

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>

int screenWidth = 640;
int screenHeight = 480;

class GLintPoint{
public:
	GLint x,y;
};

GLint xm, ym;

void drawPoint(GLint x, GLint y);

void init (void)
{
	// Dokimaste thn apenergopoihsh ths glClear(GL_COLOR_BUFFER_BIT); sthn init() kai
	// thn energopoihsh ths glClear(GL_COLOR_BUFFER_BIT); sthn display(). Ti parathreite???

	glClear(GL_COLOR_BUFFER_BIT);//The buffers currently enabled for color writing 
	glClearColor (0.0, 0.0, 0.0, 0.0);	// select clearing (background) color
   	glColor3f(1.0, 1.0, 1.0); 			// initialize viewing values 
	glPointSize(4);				// set point size
	glMatrixMode(GL_PROJECTION);		
	glLoadIdentity(); 
	gluOrtho2D(0.0,(GLdouble)screenWidth,0.0,(GLdouble)screenHeight);	//define a 2-D orthographic projection matrix 
}


void display(void){
	
	//glClear(GL_COLOR_BUFFER_BIT);	//The buffers currently enabled for color writing
	glColor3f(1.0, 1.0, 1.0); // Dokimaste thn apenergopoihsh. Ti symbainei???
	/*glBegin (GL_LINES);	
	  glVertex2i(100,50);
	  glVertex2i(100,150);
	  glVertex2i(150,150);  
	  glVertex2i(250,250);
	  glVertex2i(550,350);
	  
	glEnd();*/
	glFlush();			//force execution of GL commands
	//fprintf(stdout, "xm = %d and ym = %d\n", xm,ym);
	drawPoint(xm,ym);
}

// triggered when a keyboard key is pressed
void keyEvent(unsigned char key,int x,int y){	// x,y mouse location when key pressed
  
    switch (key) {
      // Quit
    case 'Q':
    case 'q':
	exit(0);
	break;	      
    default:	
	break;
    
    }
}

void myMouse(int button, int state, GLint x, GLint y)
{
	if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		glColor3f(0.0, 0.0, 1.0);
		xm = x;
		ym= screenHeight - y;
		glutPostRedisplay();
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		glClear(GL_COLOR_BUFFER_BIT);

	glFlush();
}


int main(int argc, char** argv) 
{
	//initialize toolkit
	glutInit(&argc, argv);
	//set display mode 
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); 
	//set window size
	glutInitWindowSize (640, 480);
	//set window initial position
	glutInitWindowPosition (1, 1);
	//creates a window 
	glutCreateWindow ("glutMouseFunc2");
			
	glutDisplayFunc(display);
	
	//registers a keyboard handler
	glutKeyboardFunc(keyEvent);
	glutMouseFunc(myMouse);	
	
	//additional initializatons
	init ();
	
	//perpetual ever processing loop
	glutMainLoop(); 
	return 0;	
}      

void drawPoint(GLint x, GLint y)
{
	glColor3f(0.0, 0.0, 1.0);
		
	glBegin(GL_POINTS);
		glVertex2i(x,y);
	glEnd();
	glFlush();
}