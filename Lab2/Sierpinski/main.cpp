#include <GL/glut.h>
#include <stdlib.h>

class GLintPoint {
public:
    GLint x, y;
};

// Global array of 3 points forming the triangle
GLintPoint T[3] = { {10, 10}, {300, 30}, {200, 300} };

void drawDot(GLint x, GLint y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

int random(int m) {
    return rand() % m;
}

void sierpinski() {
    GLintPoint point = T[random(3)]; // Select a random initial point
    drawDot(point.x, point.y); // Draw the initial point

    for (int i = 0; i < 1000; i++) { // Generate 1000 points
        int j = random(3); // Pick one of the triangle's vertices at random
        point.x = (point.x + T[j].x) / 2; // Move halfway towards the selected vertex
        point.y = (point.y + T[j].y) / 2;
        drawDot(point.x, point.y); // Draw the next point
    }
    glFlush(); // Force the execution of OpenGL commands
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT); // Clear the window
    sierpinski();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Set the background color to white
    glColor3f(0.0, 0.0, 0.0); // Set the drawing color to black
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluOrtho2D(0.0, 400.0, 0.0, 400.0); // Set up a 2D orthographic viewing region
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Single buffer and RGB color mode
    glutInitWindowSize(400, 400); // Set the window size
    glutInitWindowPosition(100, 100); // Set the window position
    glutCreateWindow("Sierpinski Gasket"); // Create the window with a title

    init(); // Initialize the OpenGL state
    glutDisplayFunc(display); // Register the display callback function
    glutMainLoop(); // Enter the GLUT main loop

    return 0;
}
