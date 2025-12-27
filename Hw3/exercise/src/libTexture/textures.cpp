// For loading .tga images : http://nehe.gamedev.net/wiki/NewLesson5.ashx
// For loading .bmp, .tga, .gif, .jpg : http://members.iinet.net.au/~cleathley/openGL/TextureLoader.htm

// using GLuint we create a variable for the texture, called 'mytexture'

// in the 'main' function we first set the variable 'mytexture' to the actual
// loaded images using:
// mytexture = LoadTexture( "grass.raw", 256, 256,1 ); for our first image
// mytexture = LoadTexture( "swirl.raw", 256, 256,2 ); for our second image
// where 256, 256 is the width and the height of the file respectively.
// 1, 2 are texture id. A texture "id" is just a number that you will use to access your textures

// then we enable the 2D Texturing, this is done with:
// glEnable( GL_TEXTURE_2D );
// and bind the texture with a polygon with:
// glBindTexture( GL_TEXTURE_2D, 1 ); for the first image
// This call will make texture that is associated with the id of 1 the active texture. 
// glBindTexture( GL_TEXTURE_2D, 2 ); for the second image
// This call will make texture that is associated with the id of 2 the active texture. 
// Any calls that have to do with OpenGL texture mapping will effect the active texture. 
// It is important that you remember those numbers since they will be needed again later on to actually apply the textures to geometry. 

// 
// Finally after drawing everything we need with the textures, we clear them to save
// system resources.

// ------------------------------------------------------------------------------
// Section 1: The loading of the texture:
// GLuint mytexture ;
// GLuint LoadTexture( const char * filename, int width, int height,int n  )
//{
//  unsigned char * data;
//	FILE * file;

// The following code will read in our RAW file
//	file = fopen( filename, "rb" ); // We need to open our file
//	if ( file == NULL ) return 0; // If our file is empty, set our texture to empty
//	data = (unsigned char *)malloc( width * height * 3 ); //assign the nessecary memory for the texture (every RGB image corresponds to a 3D array (width x height x 3) 
//	fread( data, width * height * 3, 1, file ); // read in our file
//	fclose( file ); //close our file, no point leaving it open

//	glGenTextures( n, &mytexture ); //then we need to tell OpenGL that we are generating a texture, n=1,2,3,4,5,....., the number of different textures we use
//	glBindTexture( GL_TEXTURE_2D, n ); //now we bind the texture that we are working with
//	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); //set texture environment parameters
// The parameter GL_MODULATE will blend the texture with whatever is underneath (e.g.color), setting it to GL_DECAL
// will tell the texture to replace whatever is on the object. GL_MODULE is useful for combining the effects of lighting with texturing.

//here we are setting what textures to use and when. The MIN filter is which quality to show
//when the texture is near the view, and the MAG filter is which quality to show when the texture
//is far from the view.

//The qualities are (in order from worst to best)
//GL_NEAREST
//GL_LINEAR
//GL_LINEAR_MIPMAP_NEAREST
//GL_LINEAR_MIPMAP_LINEAR

// The two mipmap variables only work in textures with generated mipmaps (images with smaller resolutions), so we will not use them now.


//(And if you go and use extensions, you can use Anisotropic filtering textures which are of an
//even better quality)

// We will use the following:

//	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
// Namely,we use a weighted linear average of the 2x2 array of texels that lie nearest to the center of the pixel is used, for both magnification and minification

//Here we are setting the parameter to repeat (GL_REPEAT)the texture instead of clamping (GL_CLAMP)the texture
//to the edge of our shape. 

//	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
//	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

//Generate the texture
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

/*The void glTexImage2D( GLenum target, GLint level, GLint components, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels ) 
 call is our goal.

PARAMETERS
target ? Specifies the target texture. Must be GL_TEXTURE_2D. 
 
level ? Specifies the level-of-detail number. Level 0 is the base image level. Level n is the nth mipmap reduction image. 
 
components ? Specifies the number of color components in the texture. Must be 1, 2, 3(for GL_RGB), or 4. 
 
width ? Specifies the width of the texture image. Must be 2k + 2 ( border ) for some integer k. 
 
height ? Specifies the height of the texture image. Must be 2m + 2 ( border ) for some integer m. 
 
border ? Specifies the width of the border. Must be either 0 or 1. (To be 1 image must be with border)
 
format ? Specifies the format of the pixel data. The following symbolic values are accepted: GL_COLOR_INDEX, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_RGBA, GL_LUMINANCE, and GL_LUMINANCE_ALPHA. 
 
type ? Specifies the data type of the pixel data. The following symbolic values are accepted: GL_UNSIGNED_BYTE, GL_BYTE, GL_BITMAP, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, and GL_FLOAT. 
 
pixels ? Specifies a pointer to the image data. This is the image data that will be uploaded to the video memory. Note that after your call to glTexImage2D you can free this memory since the texture is already uploaded into video memory.  
 
*/

//	free( data ); //free memory since now texture is in video memory

//	return mytexture; //return the texture data
//}


//--------------------------------------------------------------------------------
//Section 2: Cleaning Up:

//If we decide not to clean up after our work, you will find that the program will inevitably
//slow down your computer, taking more and more memory.
//void FreeTexture( int n, GLuint texture )
//{
//  glDeleteTextures( n, &mytexture ); // Delete our texture.
//}


//-------------------------------------------------------------------------------
//Section 3: Texture Coordinates (for more information see redbook: Computing Appropriate Texture Coordinates)

//This is how texture coordinates are arranged
//
//  0,1   -----   1,1
//       |     |
//       |     |
//       |     |
//  0,0   -----   1,0

// With 0,0 being the bottom left and 1,1 being the top right.

// Now the point of using the value 0,1 instead of 0,10, is so that it is mapping 1 texture to the
// coordinates. Changing that to 10 would then try to map 10 textures to the one quad. Which because
// we have the repeat parameter set in our texture, it would draw 10 across and 10 down, if we had
// it clamped, we would be still drawing 1. The repeat function is good for things like
// brick walls.

// To assign texture coordinates to a vertex look at the following code:
//	glBegin (GL_POLYGON);
//	glTexCoord2d(0.0,0.0); glVertex2d(-1.0,-1.0); //with our vertices we have to assign a texcoord
//  glTexCoord2d(1.0,0.0); glVertex2d(+1.0,-1.0); //so that our texture has some points to draw to
//  glTexCoord2d(1.0,1.0); glVertex2d(+1.0,+1.0);
//  glTexCoord2d(0.0,1.0); glVertex2d(-1.0,+1.0);
//	glEnd();

//For every vertex, we are assigning a texture coordinate that corresponds with it. Keep in mind
//that this does not mean that if a vertex is a -10, 20, the texture coordinates will be -10, 20. 
// They will in fact be something along the lines of 0,1.

/* You need to enable texturing before drawing your scene. Texturing in two dimensions is enabled 
	or disabled using glEnable(GL_TEXTURE_2D) or glDisable(GL_TEXTURE_2D)
*/

/*GlHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST):
requests the use of perspective interpolation in texture
rendering.
*/

//------------------------------------------------------------------------------------------------------------------

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h> 
#include <stdio.h>
#include "bmp.h"

static GLuint mytexture; //the variable for our textures

//function to load the RAW file

GLuint LoadTexture( const char * filename, int width, int height, int n )
{
	unsigned char * data;
	FILE * file;

	//The following code will read in our RAW file
	file = fopen( filename, "rb" );
	if ( file == NULL ) return 0;
	data = (unsigned char *)malloc( width * height * 3 );
	fread( data, width * height * 3, 1, file );
	fclose( file );

	glGenTextures( n, &mytexture ); //generate the texture with the loaded data
	glBindTexture( GL_TEXTURE_2D, n ); //bind the texture to it's array
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); //set texture environment parameters

	//here we are setting what textures to use and when. The MIN filter is which quality to show
	//when the texture is near the view, and the MAG filter is which quality to show when the texture
	//is far from the view.

	//The qualities are (in order from worst to best)
	//GL_NEAREST
	//GL_LINEAR
	//GL_LINEAR_MIPMAP_NEAREST
	//GL_LINEAR_MIPMAP_LINEAR

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	//Here we are setting the parameter to repeat the texture instead of clamping the texture
	//to the edge of our shape. This has a meanining when our texture coordinates are not in [0,1], but in e.g [0,2], or [0,3], etc. 
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	//Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free( data ); //free the texture
	return mytexture; //return whether it was successfull
}

GLuint LoadTextureBMP( char * filename, int width, int height, int n )
{
	unsigned char * data;
	bitmapFileHeader_t bitmapFileHeader;
	bitmapInfoHeader_t bitmapInfoHeader;
	int bytesPP, byteWidth, padWidth, padding;

	//The following code will read in our Bitmap file

	data = LoadBitmapFile( filename, &bitmapFileHeader, &bitmapInfoHeader );

	bytesPP = bitmapInfoHeader.biBitCount/8;
	byteWidth = bytesPP*(bitmapInfoHeader.biWidth);
	padWidth  = byteWidth;
	
	while(padWidth%4!=0) {
		padWidth++;
	}
	
	padding   = padWidth - byteWidth;
	
	/* The lines of a bitmap image are stored in reverse order so we have to reverse the image */
	reverseImage( data, byteWidth, bitmapInfoHeader.biHeight, padding );
	
	glGenTextures( n, &mytexture ); //generate the texture with the loaded data
	glBindTexture( GL_TEXTURE_2D, n ); //bind the texture to it's array
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); 
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	//Here we are setting the parameter to repeat the texture instead of clamping the texture
	//to the edge of our shape. 
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	//Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free( data ); //free the texture
	return mytexture; //return whether it was successfull
}

void FreeTexture( int n, GLuint textureName )
{
  glDeleteTextures( n, &textureName ); 
  
}

//This is how texture coordinates are arranged
//
//  0,1   -----   1,1
//       |     |
//       |     |
//       |     |
//  0,0   -----   1,0

// With 0,0 being the bottom left and 1,1 being the top right.

// Now the point of using the value 0,1 instead of 0,10, is so that it is mapping 1 texture to the
// coordinates. Changing that to 10 would then try to map 10 textures to the one quad. Which because
// I have the repeat parameter set in our texture, it would draw 10 across and 10 down, if we had
// it clamped, we would be still drawing 1. The repeat function is good for things like
// brick walls.

void square (void) {
	glBindTexture( GL_TEXTURE_2D, 1 ); //bind our first texture to our shape
	//glColor3f(1.0,1.0,1.0);
	//glColor3f(0.5,0.5,0.0);
	glBegin (GL_POLYGON);
	glTexCoord2d(0.0,0.0); glVertex3d(-1.5,-1.5,0.0); //with our vertices we have to assign a texcoord
    //glTexCoord2d(0.1,0.0); glVertex3d(0.0,-1.5,-1.0);
	glTexCoord2d(1.0,0.0); glVertex3d(+1.5,-1.5,-2.0); //so that our texture has some points to draw to
    glTexCoord2d(1.0,1.0); glVertex3d(+1.5,+1.5, -2.0);
	//glTexCoord2d(0.9,1.0); glVertex3d(0.0,+1.5,-1.0);
    glTexCoord2d(0.0,1.0); glVertex3d(-1.5,+1.5, 0.0);
	glEnd();
	/*glBegin (GL_POLYGON);
	glTexCoord2d(0.0,0.0); glVertex3d(-1.5,-1.5,0.0); //with our vertices we have to assign a texcoord
    glTexCoord2d(0.5,0.0); glVertex3d(+1.5,-1.5,0.0); //so that our texture has some points to draw to
    glTexCoord2d(0.5,0.2); glVertex3d(+1.5,+1.5, 0.0);// Try 0.5 instead of 2.0 in glTexCoord2d()
    glTexCoord2d(0.0,0.2); glVertex3d(-1.5,+1.5, 0.0);
	glEnd();*/

	glBindTexture( GL_TEXTURE_2D, 2 ); //bind our first texture to our second shape(a new rectangle over the first one)
	//glColor3f(1.0,1.0,1.0);
	//glColor3f(0.5,0.5,0.0);
	glBegin (GL_POLYGON);
	glTexCoord2d(0.0,0.0); glVertex3d(-1.5,+1.5,0.0); 
    glTexCoord2d(1.0,0.0); glVertex3d(+1.5,+1.5,-2.0); 
    glTexCoord2d(1.0,1.0); glVertex3d(+1.5,+2.5, -2.0); // When ratio x/y is different for the polygon and the texture (namely (x_polygon /y_polygon) != 1)there is a distortion
	//glTexCoord2d(1.0,0.5); glVertex3d(+1.5,+2.5, -2.0); // By changing the y coordinate for texture according to polygon's y coordinate (namely (x_polygon /y_polygon) = (x_texture/ytexture) =0.5)
														// there is no distortion
    glTexCoord2d(0.0,1.0); glVertex3d(-1.5,+2.5, 0.0);
	//glTexCoord2d(0.0,0.5); glVertex3d(-1.5,+2.5, 0.0);
	glEnd();

}


void triangle (void) {
	glBindTexture( GL_TEXTURE_2D, 2 ); //bind our second texture to our third shape
	//glColor3f(0.0, 1.0, 0.5);
	glBegin (GL_TRIANGLES);
	glTexCoord2d(0.0,0.0); glVertex3d(1.5,-1.5, -2.0); 
    glTexCoord2d(1.0,0.0); glVertex3d(+3.5,0.0,-2.0); 
    glTexCoord2d(0.0,1.0); glVertex3d(2.5,1.5,-2.0);
    glEnd();
}

void teapot (void) {
	glColor3f( 0.75,0.75, 0.75 );
	glPushMatrix();
	glTranslated( -0.5, 1.0, 1.0 );
	glRotatef( 45.0, 0.0, 1.0, 0.0 );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, 3 );
	glutSolidTeapot( 1.0 );
	//glutSolidCube(1.0);
	glDisable( GL_TEXTURE_2D );
	glPopMatrix();
}
void sphere (void) {
	glColor3f( 0.75,0.75, 0.75 );
	glPushMatrix();
	glTranslated( 0.5, -1.0, 1.0 );
	//glRotatef( 45.0, 0.0, 1.0, 0.0 );
/*	
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, 1 ); // try  1, 2, 3

    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluSphere(quadric, 1.0, 50, 50);
    gluDeleteQuadric(quadric);

	glDisable( GL_TEXTURE_2D );
/*/
glEnable( GL_TEXTURE_2D );
glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
glEnable(GL_TEXTURE_GEN_T);

glBindTexture( GL_TEXTURE_2D,1 );
glutSolidCube(1.0);
 
glDisable(GL_TEXTURE_GEN_S); //disable texture coordinate generation
glDisable(GL_TEXTURE_GEN_T);

glDisable( GL_TEXTURE_2D );

	glPopMatrix();
}
void init(void)
{
	glClearColor (1.0,1.0,1.0,1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}
void display (void) {
	 glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); 
	glEnable( GL_TEXTURE_2D );

	gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);/*requests the use of perspective interpolation in texture rendering.*/

	//square();
	//triangle();
	teapot();
	sphere();

	glutSwapBuffers();
	glDisable(GL_TEXTURE_2D);
}
void reshape (int w, int h) {
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
	glMatrixMode (GL_MODELVIEW);
}

int main (int argc, char **argv) {
    glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE |GLUT_DEPTH );
	glutInitWindowSize (500, 500);
	glutInitWindowPosition (100, 100);
    glutCreateWindow ("TEXTURES");
    glutDisplayFunc (display);
	glutReshapeFunc (reshape);

	//Load our texture
	mytexture = LoadTexture( "grass.raw", 256, 256, 1 );
	mytexture = LoadTexture("swirl.raw", 256, 256, 2);
	mytexture = LoadTextureBMP( "iron022.bmp", 64, 64, 3 );
	init();
    glutMainLoop ();

	//Free our texture
	FreeTexture(1,mytexture );
	FreeTexture(2,mytexture);
	FreeTexture(3,mytexture);
    return 0;
}
