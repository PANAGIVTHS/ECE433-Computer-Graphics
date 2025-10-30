#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>


//Sign function
int sign(int number) {
    return (number > 0) - (number < 0);
}

void init(){
    
    // background colour white, alpha parameter set to default
    glClearColor(1.0, 1.0, 1.0, 0.0);

    glMatrixMode(GL_PROJECTION);    
	glLoadIdentity();    
	
    // viewing area 
    gluOrtho2D(-300, 300, -300, 300); 

}

void display(int xP0, int yP0, int xP1, int yP1) {

    int dstX, dstY, numPixels, condSlope;
    int coord1, coord2, endPointCoord;
    int signCoord1, signCoord2, dstCoord1, dstCoord2;
    int error;

    // calculating distance for x 
    dstX = abs(xP1 - xP0);

    // calculating distance for y
    dstY = abs(yP1 - yP0);

    // If P0 = P1
    if (!(dstX && dstY)) {
        glBegin(GL_POINTS);
        glVertex2i(xP0, yP0);
        glEnd();
    }

    // FOR INTERPOLATION //
    // NEEDS TO BE ADRESSED FURTHER//
    // num_pixels = max(dx, dy); //

    condSlope = dstX >= dstY;

    // comments need to be added
    if(condSlope) {
        coord1 = xP0;
        coord2 = yP0;
        
        endPointCoord = xP1;
        
        signCoord1 = sign(xP1 - xP0);
        signCoord2 = sign(yP1 - yP0);
        
        dstCoord1 = dstX;
        dstCoord2 = dstY;
        
        glBegin(GL_POINTS);
        glVertex2i(coord1, coord2);
        glEnd();
    }
    else {
        coord1 = yP0;
        coord2 = xP0;
        
        endPointCoord = yP1;
        
        signCoord1 = sign(yP1 - yP0);
        signCoord2 = sign(xP1 - xP0);
        
        dstCoord1 = dstY;
        dstCoord2 = dstX;
        
        glBegin(GL_POINTS);
        glVertex2i(coord2, coord1);
        glEnd();
        
    }

    // calculating the error 
    error = (2 * dstCoord2) - dstCoord1;

    while (coord1 != endPointCoord) {
        error += 2 * dstCoord2;
        coord1 += signCoord1;
    
        if (error >= 0) {
            error -= 2 *dstCoord1;
            coord2 += signCoord2;
        }
    
        if (condSlope) {
            glBegin(GL_POINTS);
            glVertex2i(coord1, coord2);
            glEnd();
        }
        else {
            glBegin(GL_POINTS);
            glVertex2i(coord2, coord1);
            glEnd();
        }
    }
}



// function that terminates the program with the press of button Q/q
void termination (unsigned char key, int x, int y){

    switch (key) {
        case 'Q':
        case 'q':
            exit(0);
            break;
        default:
            break;
    }
}


int main(int argc, char** argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    // set window size
    glutInitWindowSize(600, 600); 

    // set window initial position
    glutInitWindowPosition(10, 10);
    
    // create window with the given title
    glutCreateWindow("Team 1 - Assignment 1 - Exercise 4"); 

    glutDisplayFunc();

}