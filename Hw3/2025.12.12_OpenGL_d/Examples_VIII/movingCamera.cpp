// Ylopoihsh kinoumenhs kameras.



// Mege8ynsh (gluPerspective, glViewport)
// Gia beltisto prosdiorismo tou Viewing Volume me th gluPerspective deite to paradeigma ypologismou sto 
// redbook, sel 88/616 kai 94- 95/616

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

# define FRONT 56 // 8
# define BACK  50 // 2
# define RIGHT 54 // 6
# define LEFT  52 // 4

double angle=0.0;
int w=640, h=480;
//int w=1280, h=960;

// Camera's position se sxesh me to Viewing Volume
	double eyeX =0.0, eyeY =1.6, eyeZ = 8.0;//eyeY =1.6, Epeidh to synoliko ycos tou spitiou einai 3.20 kai h 
	// arxh toy Pagkosmiou Systhmatos Syntetagm;envn e;inei sto kentro tou dapedou
	//Dokimaste to: double eyeX =0.0, eyeY =0.0, eyeZ = 8.0;
// Direction
	double lookXAt =0.0, lookYAt= 1.6, lookZAt = -6.0;//lookYAt= 1.6, Epeidh to synoliko ycos tou spitiou einai 3.20
	//double lookXAt =0.0, lookYAt= 0.0, lookZAt = -6.0;//lookYAt= 1.6
void house(void)
{
	
	// Back wall
	glColor3f(0.9,0.9, 0.0);
	glPushMatrix();
	glTranslated(0.0, 1.70, -2.90);
	glScaled(8.0, 3.0, 0.20);
	glutSolidCube(1.0);
	glPopMatrix();
	
	//Left wall
	glColor3f(1.0,1.0, 0.2);
	glPushMatrix();
	glTranslated(-3.90, 1.70, 0.10);
	glScaled(0.20, 3.0, 5.80);
	glutSolidCube(1.0);
	glPopMatrix();

	//Right wall
	glColor3f(0.7,0.8, 0.5);
	glPushMatrix();
	glTranslated(3.90, 1.70, 0.10);
	glScaled(0.20, 3.0, 5.80);
	glutSolidCube(1.0);
	glPopMatrix();

	// Floor
	glColor3f(0.9,0.7, 0.5);
	glPushMatrix();
	glTranslated(0.0, 0.10, 0.0);
	glScaled(8.0, 0.20, 6.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glFlush();

}


void myInit (void)
{
	
	glClearColor(0.8, 0.8, 0.8, 0.0);    
	glPointSize(3);
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE); 
	glEnable(GL_DEPTH_TEST); 

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (45, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
	glMatrixMode (GL_MODELVIEW);
}

void display(void)
{
	glViewport(w/4,h/4, w/2, h/2);
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

		glLoadIdentity(); 
		gluLookAt (eyeX, eyeY, eyeZ, lookXAt, lookYAt, lookZAt, 0.0, 1.0, 0.0); //perspective

		house();
glutSwapBuffers();
}

// Movement of the Camera

void moveCamera(int key)
{
	// m: to mhkos tou dianysmatos (lookAt-eye), sto XZ epipedo
	double m = sqrt((lookXAt -eyeX)*(lookXAt-eyeX)+(lookZAt -eyeZ)*(lookZAt-eyeZ));

	// To bhma sthn kateyqynsh x kai to bhma sthn kateyqynsh z ypologizontai me th bohqeia 
	// tou hmitonou kai tou synimhtonou kai tou bhmatos pou epiqymoume sthn kateyqynsh tou dianysmatos (lookAt-eye), edw 0.05

	double stepX = 0.05* (lookXAt -eyeX)/m;
	double stepZ = 0.05* (lookZAt -eyeZ)/m;

	if (key == FRONT)
	{
		eyeX = eyeX + stepX;
		eyeZ = eyeZ + stepZ;

	if (lookZAt < eyeZ)
			lookZAt -=stepZ;
		else
			lookZAt +=stepZ;
	}	
	
	
	else if (key == BACK)
	{
		eyeX = eyeX - stepX;
		eyeZ = eyeZ - stepZ;
		if (lookZAt < eyeZ)
			
			lookZAt +=stepZ;
		else
			
			lookZAt -=stepZ; 
	}
	else if (key == LEFT) {	//

		if ((lookXAt <= eyeX) && (lookZAt < eyeZ)) {

			lookXAt -= 0.1;
			lookZAt += 0.1;
		
		}
		else if ((lookXAt < eyeX) && (lookZAt >= eyeZ)) {

			lookXAt += 0.1;
			lookZAt += 0.1;
			
		}
		else if ((lookXAt >= eyeX) && (lookZAt > eyeZ)) {

			lookXAt += 0.1;
			lookZAt -= 0.1;
			
		}
		else if ((lookXAt > eyeX) && (lookZAt <= eyeZ)) {

			lookXAt -= 0.1;
			lookZAt -= 0.1;
			
		}
	}
	else if (key == RIGHT) {	//

		if ((lookXAt >= eyeX) && (lookZAt < eyeZ)) {

			lookXAt += 0.1;
			lookZAt += 0.1;
			//lookXAt = (lookXAt <= 10.0)?lookXAt:10.0;
		}
		else if ((lookXAt > eyeX) && (lookZAt >= eyeZ)) {

			lookXAt -= 0.1;
			lookZAt += 0.1;
			
		}
		else if ((lookXAt <= eyeX) && (lookZAt > eyeZ)) {

			lookXAt -= 0.1;
			lookZAt -= 0.1;
			
		}
		else if ((lookXAt < eyeX) && (lookZAt <= eyeZ)) {

			lookXAt += 0.1;
			lookZAt -= 0.1;
			
		}		
	}
}

// H synarthsh myKeyBoard " diabazei" tis eisodous tou xrhsth apo to plhktrologio
void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
        exit(0);
        break;
	case 56: // FRONT
		moveCamera(key);
		break;
	case 50: // BACK
		moveCamera(key);
		break;
	
	case 54: // RIGHT
		moveCamera(key);
		break;

	case 52: // LEFT
		moveCamera(key);
		break;
	}
	glutPostRedisplay();
}


int main(int argc, char **argv)
{
	
	// initialize toolkit
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);

	glutInitWindowSize (w, h);
	
	glutInitWindowPosition(1,1);
		
	glutCreateWindow ("moving camera");
	
		
	glutDisplayFunc(display);

	glutKeyboardFunc(myKeyboard);
	printf("8-> FRONT, 2-> BACK \n");
	printf("6-> RIGHT, 4-> LEFT\n");

	myInit();
	
	glutMainLoop();

	return 0;
}


