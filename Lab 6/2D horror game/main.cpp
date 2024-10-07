#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include "pixMap.h"  // Include the pixMap.h header file

using namespace std;

RGBApixmap backgroundPixmap;
int screenWidth = 940;
int screenHeight = 580;

class Point2 {
public:
    Point2() : x(0.0f), y(0.0f), size(1.0f) {}
    Point2(float xx, float yy, float sz = 1.0f) : x(xx), y(yy), size(sz) {}

    void set(float xx, float yy) { x = xx; y = yy; }
    void setSize(float sz) { size = sz; }
    float getX() const { return x; }
    float getY() const { return y; }
    float getSize() const { return size; }

private:
    float x, y;
    float size;
};

class Ghost {
private:
    RGBApixmap pix[2];
    float directionX, directionY;

public:
    float pos_X, pos_Y;
    bool is_moving;
    float speed;

    Ghost() : pos_X(0), pos_Y(0), is_moving(true), speed(0.1f), directionX(1.0f), directionY(1.0f) {
        pix[1].readBMPFile("ghost5.bmp", 1);
        pix[1].setChromaKey(255, 255, 255);
    }

    Ghost(float x, float y) : pos_X(x), pos_Y(y), is_moving(true), speed(0.2f), directionX(1.0f), directionY(1.0f) {
        pix[1].readBMPFile("ghost5.bmp", 1);
        pix[1].setChromaKey(255, 255, 255);
    }

    void changePosition(float dx, float dy) {
        pos_X += dx;
        pos_Y += dy;
    }

    void setPosition(float x, float y) {
        pos_X = x;
        pos_Y = y;
    }

    float getX() const {
        return pos_X;
    }

    float getY() const {
        return pos_Y;
    }

    void move() {
        if (!is_moving) return;

        pos_X += directionX * speed;
        pos_Y += directionY * speed;

        if (pos_X < 0 || pos_X > screenWidth - 50) {
            directionX = -directionX;
        }
        if (pos_Y < 0 || pos_Y > screenHeight - 50) {
            directionY = -directionY;
        }
    }

    void render() {
        glRasterPos2i(static_cast<int>(pos_X), static_cast<int>(pos_Y));
        pix[1].mDraw();
    }

    void teleport() {
        pos_X = rand() % (screenWidth - 50); // Random x position
        pos_Y = rand() % (screenHeight - 50); // Random y position
    }
};

// New class for the second ghost using OpenGL shapes
class ShapeGhost {
public:
    float pos_X, pos_Y;
    float width, height;
    float directionX, directionY;
    bool is_moving;
    float speed;

    ShapeGhost() : pos_X(0), pos_Y(0), width(70.0f), height(60.0f), is_moving(true), speed(0.1f), directionX(1.0f), directionY(1.0f) {}

    ShapeGhost(float x, float y) : pos_X(x), pos_Y(y), width(70.0f), height(60.0f), is_moving(true), speed(0.2f), directionX(1.0f), directionY(1.0f) {}

    void changePosition(float dx, float dy) {
        pos_X += dx;
        pos_Y += dy;
    }

    void setPosition(float x, float y) {
        pos_X = x;
        pos_Y = y;
    }

    float getX() const {
        return pos_X;
    }

    float getY() const {
        return pos_Y;
    }

    void move() {
        if (!is_moving) return;

        pos_X += directionX * speed;
        pos_Y += directionY * speed;

        if (pos_X < width / 2 || pos_X > screenWidth - width / 2) {
            directionX = -directionX;
        }
        if (pos_Y < height / 2 || pos_Y > screenHeight - height / 2) {
            directionY = -directionY;
        }
    }

    // Draw the rounded top of the ghost (half-circle)
    void drawRoundedTop(float radius) {
        glBegin(GL_POLYGON);
        for (int i = 0; i <= 50; i++) {
            float angle = 3.14159f * i / 50;  // Only the top half of the circle
            float dx = radius * cosf(angle);
            float dy = radius * sinf(angle);
            glVertex2f(dx, dy + height / 2);  // Position it at the top
        }
        glEnd();
    }

    // Draw the center body of the ghost
    void drawCenterBody(float width, float height) {
        glBegin(GL_POLYGON);
        glVertex2f(-width / 2, height / 2);  // Top-left corner
        glVertex2f(width / 2, height / 2);   // Top-right corner
        glVertex2f(width / 2, -height / 2);  // Bottom-right corner
        glVertex2f(-width / 2, -height / 2); // Bottom-left corner
        glEnd();
    }

    // Draw the wavy bottom of the ghost (simple sinusoidal wave)
    void drawWavyBottom(float width, float height) {
        glBegin(GL_POLYGON);
        glVertex2f(-width / 2, -height / 2);  // Bottom-left corner

        for (int i = 0; i <= 50; i++) {
            float x = -width / 2 + i * (width / 50);
            float wave = sinf(i * 0.3f) * 5.0f;  // Gentle wave
            glVertex2f(x, -height / 2 + wave);
        }

        glVertex2f(width / 2, -height / 2);  // Bottom-right corner
        glEnd();
    }

    // Draw the eyes (small black circles)
    void drawEye(float x, float y, float eyeRadius) {
        glBegin(GL_POLYGON);
        for (int i = 0; i < 100; i++) {
            float angle = 2.0f * 3.14159f * i / 100;
            float dx = eyeRadius * cosf(angle);
            float dy = eyeRadius * sinf(angle);
            glVertex2f(x + dx, y + dy);
        }
        glEnd();
    }

    // Draw the mouth (small circle)
    void drawMouth(float x, float y, float mouthRadius) {
        glBegin(GL_POLYGON);
        for (int i = 0; i < 100; i++) {
            float angle = 2.0f * 3.14159f * i / 100;
            float dx = mouthRadius * cosf(angle);
            float dy = mouthRadius * sinf(angle);
            glVertex2f(x + dx, y + dy);
        }
        glEnd();
    }

    // Render the ghost by combining the body parts
    void render() {
        glPushMatrix();
        glTranslatef(pos_X, pos_Y, 0);

        glColor3f(1.0f, 1.0f, 1.0f);  // White color for the ghost

        // Draw the body (rounded top and wavy bottom)
        drawRoundedTop(width / 2);   // Half-circle top
        drawCenterBody(width, height);
        drawWavyBottom(width, height);  // Wavy bottom

        // Draw the eyes (black circles)
        glColor3f(0.0f, 0.0f, 0.0f);  // Black color for the eyes
        drawEye(-15.0f, 25.0f, 6.0f);  // Left eye
        drawEye(15.0f, 25.0f, 6.0f);   // Right eye

        // Draw the mouth (small circle)
        drawMouth(0.0f, 0.0f, 6.0f);

        glPopMatrix();
    }

    // Teleport function for random positioning
    void teleport() {
        pos_X = rand() % (screenWidth - 50); // Random x position
        pos_Y = rand() % (screenHeight - 50); // Random y position
    }
};



Ghost ghost(100.0f, 100.0f);
ShapeGhost randomGhost(300.0f, 300.0f); // Second ghost using shapes
float teleportDelay = 1.0f; // Delay in seconds
float lastTeleportTime = 0.0f; // Last teleport time

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glRasterPos2f(0, 0);
    backgroundPixmap.mDraw();

    ghost.move();
    ghost.render();

    // Get the current time
    static float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

    // Move the second ghost randomly at intervals
    float elapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f - lastTeleportTime;
    if (elapsedTime >= teleportDelay) {
        randomGhost.teleport();
        lastTeleportTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Update the last teleport time
    }
    randomGhost.render();

    glutSwapBuffers();
}

void initOpenGL() {
    glViewport(0, 0, screenWidth, screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, screenWidth, 0, screenHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_EQUAL, 1.0);

    backgroundPixmap.readBMPFile("bgimg1.bmp", 1);
    backgroundPixmap.setChromaKey(255, 0, 255);
}

int main(int argc, char **argv) {
    srand(static_cast<unsigned>(time(0))); // Seed for random number generation

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("Ghost Game");

    initOpenGL();

    glutDisplayFunc(display);
    PlaySound(TEXT("scary-horror-sound.wav"), NULL, SND_ASYNC);
    glutIdleFunc(display);

    glutMainLoop();
    return 0;
}
