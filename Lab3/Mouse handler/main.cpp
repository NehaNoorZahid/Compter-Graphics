#include <iostream>
#include <string>
#include <GL/glut.h>
#include <vector>

using namespace std;

int screenWidth = 800;
int screenHeight = 600;

float clearColorRed = 0.0f;
float clearColorGreen = 0.0f;
bool isPaused = false;
bool isDrawing = false;
bool drawMountains = false;

struct Point {
    int x;
    int y;
    Point(int _x, int _y) : x(_x), y(_y) {}
};

vector<Point> points;
vector<vector<Point>> lines; // Store lines as a vector of points

void reshape(int width, int height) {
    screenWidth = width;
    screenHeight = height;

    glViewport(0, 0, screenWidth, screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, screenWidth, 0, screenHeight);
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN && !isPaused) {
        if (button == GLUT_LEFT_BUTTON) {
            points.push_back(Point(x, screenHeight - y)); // Store clicked point
        }
        glutPostRedisplay();
    }

    if (state == GLUT_DOWN && !isPaused) {
        if (button == GLUT_LEFT_BUTTON) {
            isDrawing = true; // Start drawing
            lines.emplace_back(); // Start a new line
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            isDrawing = false; // Stop drawing
        }
        glutPostRedisplay();
    }
}

void mousemove(int x, int y) {
    if (isDrawing) {
        lines.back().push_back(Point(x, screenHeight - y)); // Add point to current line
        glutPostRedisplay();
    }
}

void drawMountainRanges() {
    int mountainCount = 20;
    int mountainWidth = screenWidth / mountainCount;
    int mountainHeight = 100;
    glColor3f(0.55f, 0.27f, 0.07f); // Set mountain color to brown

    for (int i = 0; i < mountainCount; ++i) {
        glBegin(GL_TRIANGLES);
        glVertex2i(i * mountainWidth, 0);
        glVertex2i(i * mountainWidth + mountainWidth / 2, mountainHeight);
        glVertex2i(i * mountainWidth + mountainWidth, 0);
        glEnd();
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'q':
    case 'Q':
        exit(0);
    case 'f':
    case 'F':
        clearColorRed = 0.0f;   // Set foreground color to green
        clearColorGreen = 1.0f; // Set foreground color to green
        glutPostRedisplay();
        break;
    case 'm':
    case 'M':
        drawMountains = !drawMountains;
        glutPostRedisplay();
        break;
    case 'c':
    case 'C':
        // Clear screen
        points.clear();
        lines.clear();
        glutPostRedisplay();
        break;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(clearColorRed, clearColorGreen, 0.0f, 1.0f);

    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 0.0f); // Set point color to yellow
    for (const Point& p : points) {
        glVertex2i(p.x, p.y); // Draw each stored point
    }
    glEnd();

    if (drawMountains) {
        drawMountainRanges();
    }

    // Draw lines for free-hand drawing
    glColor3f(0.5f, 0.0f, 0.5f); // Set line color to purple
    for (const auto& line : lines) {
        glBegin(GL_LINE_STRIP);
        for (const Point& p : line) {
            glVertex2i(p.x, p.y);
        }
        glEnd();
    }

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("OpenGL Mouse Handler Example");

    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mousemove); // Register mouse move callback for free-hand drawing
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}
