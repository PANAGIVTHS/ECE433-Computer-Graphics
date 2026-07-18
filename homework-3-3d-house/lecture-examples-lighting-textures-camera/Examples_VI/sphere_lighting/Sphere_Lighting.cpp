
/*
 LIGTHING: 
 Kinds of lighting
 Enable Lighting calculations
 Enable light sourses.
 Directional light, spot light, general ambient light
 Anaqesh idiothtwn ylikwn
*/

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>


/*  Initialize material property, light source, lighting model,
 *  and depth buffer.
 */
void init(void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH); // normals are computed per vertex
	//glShadeModel (GL_FLAT); // normals are computed per surface
	
	GLfloat mat_amb_diff[] = {1.0, 1.0, 1.0, 1.0};
	//GLfloat mat_amb_diff[] = {1.0, 1.0, 0.0, 1.0}; //Try red or green material color and white light
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat mat_shininess[] = { 20.0 };
	GLfloat light0_position[] = { 1.0, 1.0, 1.0, 0.0 }; // directional light 
	// Oi syntetagmenes ths qeshs orizoun thn kateyqynsh apo thn opoia erxetai to fws.
	// H fwteinh phgh brisketai sto apeiro
	//GLfloat light0_position[] = { 1.0, 1.0, 1.0, 1.0 };// positional light
	//Oi syntetagmenes ths qeshs (omogeneis) orizoun th qesh sthn opoia brisketai h fwteinh phgh
	// Apoqhkeyontai stis eye coordinates (GL_MODELVIEW)
	GLfloat LIGHT0_light[] = {1.0,1.0,1.0,1.0};
	GLfloat light0_ambient[] = {0.0, 0.5, 0.2, 1.0}; // Because of light0

	
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR,mat_specular );
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	// A derectional light (try it as positional)
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);// It can be different from diffuse light
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LIGHT0_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LIGHT0_light); // Prepei to fws gia specular na exei to idio xrwma me to diffuse,dhladh to xrwma ths fwteinhs phghs
	
	
	GLfloat lmodel_ambient[] = {0.3, 0.5, 0.9, 1.0}; // General, not because of a specific light source
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	//glDisable(GL_LIGHTING); //try to remove comments
	//glDisable(GL_LIGHT0); //try to remove comments
	glutSolidSphere (1.0, 20, 16);
	glFlush ();

}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w,
         1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
   else
      glOrtho (-1.5*(GLfloat)w/(GLfloat)h,
         1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
