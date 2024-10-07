#include<windows.h>
#include <GL/glut.h>

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);    // Set display window color to black

    glMatrixMode(GL_PROJECTION);         // Set projection parameters
    gluOrtho2D(0.0, 60.0, 0.0, 60.0);    // Adjust the coordinate system to match the grid in the image
}

void drawShapes(void)
{
    glClear(GL_COLOR_BUFFER_BIT);    // Clear display window

    // Draw the roof (triangle)
    glColor3f(0.6, 0.3, 0.2);        // Brown color for the roof
    glBegin(GL_TRIANGLES);
        glVertex2i(10, 25);          // Left corner of the roof
        glVertex2i(30, 50);          // Top of the roof
        glVertex2i(50, 25);          // Right corner of the roof
    glEnd();

    // Draw the main body (rectangle)
    glColor3f(1.0, 0.5, 0.0);        // Orange color for the main body
    glBegin(GL_POLYGON);
        glVertex2i(15, 5);           // Bottom-left
        glVertex2i(45, 5);           // Bottom-right
        glVertex2i(45, 25);          // Top-right
        glVertex2i(15, 25);          // Top-left
    glEnd();

    // Draw the door (rectangle)
    glColor3f(1.0, 1.0, 0.0);        // Yellow color for the door
    glBegin(GL_POLYGON);
        glVertex2i(20, 5);           // Bottom-left
        glVertex2i(30, 5);           // Bottom-right
        glVertex2i(30, 15);          // Top-right
        glVertex2i(20, 15);          // Top-left
    glEnd();

    glFlush();    // Process all OpenGL routines
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);                        // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  // Set display mode

    glutInitWindowPosition(300, 50);              // Set window position
    glutInitWindowSize(500, 500);                 // Set window size
    glutCreateWindow("House Drawing");            // Create display window

    init();                                       // Execute initialization procedure
    glutDisplayFunc(drawShapes);                  // Send graphics to display window
    glutMainLoop();                               // Display everything and wait

    return 0;
}
