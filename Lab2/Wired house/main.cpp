#include <GL/glut.h>

// Function to draw the hard-wired house
void hardwiredHouse(void) {
    // Draw the outer shell of the house
    glBegin(GL_LINE_LOOP);
    glVertex2i(40, 40);   // Bottom-left corner
    glVertex2i(40, 90);   // Top-left corner
    glVertex2i(70, 120);  // Peak of the roof
    glVertex2i(100, 90);  // Top-right corner
    glVertex2i(100, 40);  // Bottom-right corner
    glEnd();

    // Draw the chimney
    glBegin(GL_LINE_STRIP);
    glVertex2i(50, 100);  // Bottom of the chimney
    glVertex2i(50, 120);  // Top-left of the chimney
    glVertex2i(60, 120);  // Top-right of the chimney
    glVertex2i(60, 110);  // Bottom-right of the chimney
    glEnd();

    // Draw the door
    glBegin(GL_LINE_LOOP);
    glVertex2i(60, 40);   // Bottom-left corner of the door
    glVertex2i(60, 65);   // Top-left corner of the door
    glVertex2i(80, 65);   // Top-right corner of the door
    glVertex2i(80, 40);   // Bottom-right corner of the door
    glEnd();

    // Draw the window
    glBegin(GL_LINE_LOOP);
    glVertex2i(85, 75);   // Bottom-left corner of the window
    glVertex2i(85, 85);   // Top-left corner of the window
    glVertex2i(95, 85);   // Top-right corner of the window
    glVertex2i(95, 75);   // Bottom-right corner of the window
    glEnd();

    glFlush();  // Ensure all OpenGL commands are executed
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the display
    hardwiredHouse();              // Call the function to draw the house
}

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);  // Set background color to white
    glColor3f(0.0, 0.0, 0.0);          // Set the drawing color to black
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();                   // Initialize projection matrix
    gluOrtho2D(0.0, 150.0, 0.0, 150.0); // Set the coordinate system for 2D
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                           // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);     // Set display mode
    glutInitWindowSize(400, 400);                    // Set window size
    glutInitWindowPosition(100, 100);                // Set window position
    glutCreateWindow("Hardwired House");             // Create window with title
    init();                                          // Initialize OpenGL
    glutDisplayFunc(display);                        // Register display callback
    glutMainLoop();                                  // Enter the event loop
    return 0;
}
