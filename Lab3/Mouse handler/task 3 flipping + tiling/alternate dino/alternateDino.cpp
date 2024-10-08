#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <vector>

struct Point {
    GLfloat x, y;
};

std::vector<std::vector<Point>> polylines;

void drawPolylines() {
    glColor3f(0.0, 0.0, 0.0); // Set drawing color to black

    for (const auto& polyline : polylines) {
        glBegin(GL_LINE_STRIP);
        for (const auto& point : polyline) {
            glVertex2f(point.x, point.y);
        }
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int j = 0; j < 10; j++) { // 10 rows of tiles
        for (int i = 0; i < 10; i++) { // 10 columns of tiles
            glViewport(i * 64, j * 48, 64, 48);

            if ((i + j) % 2 == 0) { // Alternate tiles are flipped upside down
                glPushMatrix();
                glTranslatef(0.0, 250.0, 0.0); // Translate to bottom of tile   250 for whole dino
                glScalef(1.0, -1.0, 1.0); // Flip the drawing vertically
                glTranslatef(0.0, -250.0, 0.0); // Translate back to original position
                drawPolylines();
                glPopMatrix();
            }
            else {
                drawPolylines();
            }
        }
    }

    glFlush();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0); // Set background color to white
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

void parseData() {
    std::ifstream inFile("dino.dat");
    if (!inFile) {
        std::cerr << "Unable to open file." << std::endl;
        exit(1);
    }

    int numPolylines;
    inFile >> numPolylines;

    for (int i = 0; i < numPolylines; i++) {
        int numVertices;
        inFile >> numVertices;

        std::vector<Point> polyline;
        for (int j = 0; j < numVertices; j++) {
            Point point;
            inFile >> point.x >> point.y;
            polyline.push_back(point);
        }

        polylines.push_back(polyline);
    }

    inFile.close();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Lab Activity 03: Flipped Tiled Dinosaurs");
    parseData();
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}