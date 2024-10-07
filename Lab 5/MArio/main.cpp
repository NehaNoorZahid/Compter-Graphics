#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <GL/freeglut.h>
#include "pixMap.h"
#include "Mario.h"

Mario m(Point2(0, 0));

void myInit(void)
{
  glClearColor(0.678f, 0.847f, 0.902f, 1.0f); 
  glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_EQUAL, 1.0);
	glViewport(0, 0, 640, 480);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0,640,0,480);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  m.changeMode(Mario::STAY);
  
}


void display(void)
{
  
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
     m.render();
	   glFlush();
  
}

void myKB_Handler(unsigned char key, int mx, int my)
{
  switch (key)
  {
  case 'r':

        m.changeMode(Mario::RUN);
        break;

  case 'j':

         m.changeMode(Mario::JUMP);
         break;
  }

glutPostRedisplay();
}

void pressKeySpecial(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        std::cout << "Left key pressed" << std::endl;  // Debug message
        m.setPosition(-2,0);
        m.changePosition(-2, 0); // Move left by 2 units
        m.changeMode(Mario::RUN); // Mario should be running
        break;
    
    case GLUT_KEY_RIGHT:
        std::cout << "Right key pressed" << std::endl;  // Debug message
        m.setPosition(2,0);
        m.changePosition(2, 0); // Move right by 2 units
        m.changeMode(Mario::RUN); // Mario should be running
        break;
    
    case GLUT_KEY_UP:
        std::cout << "Up key pressed" << std::endl;  // Debug message
        m.setPosition(0,2);
        m.changePosition(0, 2); // Move up by 2 units
        m.changeMode(Mario::JUMP); // Mario should jump
        break;
    
    case GLUT_KEY_DOWN:
        std::cout << "Down key pressed" << std::endl;  // Debug message
        m.setPosition(0,-2);
        m.changePosition(0, -2); // Move down by 2 units
        m.changeMode(Mario::JUMP); // Mario should be in the jump mode
        break;
    }
    glutPostRedisplay();
}


void releaseKeySpecial(int key, int x, int y)
{
    // Stop Mario's movement when any key is released
    switch (key)
    {
    case GLUT_KEY_LEFT:
    case GLUT_KEY_RIGHT:
    case GLUT_KEY_UP:
    case GLUT_KEY_DOWN:
        // Reset Mario's position change or set to a stationary mode
        m.changePosition(0, 0); // Stop movement
        m.changeMode(Mario::STAY); // Assuming Mario should stay in place
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(640, 480);
  glutInitWindowPosition(10, 10);
  glutCreateWindow("MY Mario");
  
  
  glutDisplayFunc(display);
  glutSpecialFunc(pressKeySpecial);
  glutSpecialUpFunc(releaseKeySpecial);
  glutKeyboardFunc(myKB_Handler);
  

  myInit();
  //glutIdleFunc(display);
  //glutTimerFunc(500,myTimer,1);
  glutMainLoop();
}

