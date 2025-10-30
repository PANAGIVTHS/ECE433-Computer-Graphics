#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>


void init(){

}


void (){

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