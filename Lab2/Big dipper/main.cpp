#include <GL/glut.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Set point size
    glPointSize(10.0);

    // Set dot color to black
    glColor3f(0.0f, 0.0f, 0.0f);

    // Draw points for the Big Dipper
    glBegin(GL_POINTS);

    // Define the coordinates of each star (similar to the image's positions)
    glVertex2f(289.0f / 400.0f, 190.0f / 400.0f);  // Dubhe
    glVertex2f(320.0f / 400.0f, 128.0f / 400.0f);  // Merak
    glVertex2f(239.0f / 400.0f, 67.0f / 400.0f);   // Phecda
    glVertex2f(194.0f / 400.0f, 101.0f / 400.0f);  // Megrez
    glVertex2f(129.0f / 400.0f, 83.0f / 400.0f);   // Alioth
    glVertex2f(74.0f / 400.0f, 74.0f / 400.0f);    // Alcor
    glVertex2f(20.0f / 400.0f, 10.0f / 400.0f);    // Alkaid

    glEnd();

    glFlush();
}

void init() {
    // Set background color to white
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Set the projection and modelview matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);  // Set the coordinate system for 2D
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutCreateWindow("Big Dipper");
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
