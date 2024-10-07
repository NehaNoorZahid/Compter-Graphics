#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include "pixMap.h"  // Include the pixMap.h header file

using namespace std;

RGBApixmap backgroundPixmap;
int screenWidth = 940;
int screenHeight = 580;
bool gameOver = false; // Flag to indicate game over

class Point2 {
public:
    Point2() : x(0.0f), y(0.0f), size(1.0f) {}  // Constructor 1
    Point2(float xx, float yy, float sz = 1.0f) : x(xx), y(yy), size(sz) {}  // Constructor 2

    void set(float xx, float yy) { x = xx; y = yy; }
    void setSize(float sz) { size = sz; }
    float getX() const { return x; }
    float getY() const { return y; }
    float getSize() const { return size; }

    void draw() const {
        if (gameOver) return; // Don't render if game over

        glPushMatrix();
        glTranslatef(x, y, 0.0);
        glScalef(size, size, 1.0);  // Scale the drawing

        // Set color
        glColor3f(1.0, 0.0, 0.0); 

        // Draw the shape (for bullets)
        glBegin(GL_TRIANGLES);
        glVertex2f(0, 0);          // Top vertex
        glVertex2f(-10, -15);      // Bottom left vertex
        glVertex2f(10, -15);       // Bottom right vertex
        glEnd();

        glBegin(GL_POLYGON);
        glVertex2f(10, -12);       // Top right
        glVertex2f(-10, -12);      // Top left
        glVertex2f(-10, -30);      // Bottom left
        glVertex2f(10, -30);       // Bottom right
        glEnd();

        glPopMatrix();
    }

private:
    float x, y;
    float size;
};

class Spider {
private:
    enum State { ALIVE, DEAD } state;
    RGBApixmap pix[2];  // Use the RGBApixmap from pixMap.h
    float directionX, directionY; // Direction of movement

public:
    float pos_X, pos_Y;
    bool is_moving;
    float sspeed;  // Speed of the spider
    float theta;
    float rate_tChange;

    // Default constructor
    Spider() : pos_X(0), pos_Y(0), is_moving(true), sspeed(0.1f), theta(0.0f), rate_tChange(0.01f), directionX(1.0f), directionY(1.0f) {
        // Default image filenames
        string fname[2] = { "spider.bmp", "splash.bmp" };
        for (int i = 0; i < 2; i++) {
            pix[i].readBMPFile(fname[i].c_str(), 1);  // Read BMP files
            pix[i].setChromaKey(255, 255, 255);       // Set chroma key if necessary (for transparency)
        }
    }

    // Constructor with position
    Spider(float x, float y) : pos_X(x), pos_Y(y), is_moving(true), sspeed(0.2f), theta(0.0f), rate_tChange(0.01f), directionX(1.0f), directionY(1.0f) {
        // Image filenames
        string fname[2] = { "spider.bmp", "splash.bmp" };
        for (int i = 0; i < 2; i++) {
            pix[i].readBMPFile(fname[i].c_str(), 1);  // Read BMP files
            pix[i].setChromaKey(255, 255, 255);       // Set chroma key if necessary (for transparency)
        }
    }

    void setState(int i) {
        if (i == 1)
            this->state = DEAD;
        else {
            this->state = ALIVE;
        }
    }

    int getState() {
        return (this->state == DEAD) ? 1 : 0;
    }

    void changePosition(float dx, float dy) {
        this->pos_X += dx;  
        this->pos_Y += dy;
    }

    void die() {
        this->is_moving = false;
        this->state = DEAD;
        this->pos_X -= 15.0f;  // Move the spider slightly to the left when it dies
        gameOver = true;  // Set the game over flag
    }

    void setPosition(float x, float y) {
        pos_X = x;
        pos_Y = y;
    }

    float spider_getX() const {
        return pos_X;
    }

    float spider_getY() const {
        return pos_Y;
    }

    void move() {
        if (!is_moving || gameOver) return; // Don't move if game is over

        // Update position with constant movement
        pos_X += directionX * sspeed;

        // Bounce off the screen edges
        if (pos_X < 0 || pos_X > screenWidth - 50) {
            directionX = -directionX; // Reverse X direction
        }
    }

    void render() {
        if (this->is_moving) {
            glRasterPos2i(static_cast<int>(this->pos_X), static_cast<int>(this->pos_Y));
            pix[0].mDraw();  // Draw spider image
        } else {
            glRasterPos2i(static_cast<int>(this->pos_X), static_cast<int>(this->pos_Y));
            pix[1].mDraw();  // Draw splash image when spider is dead
        }
    }
};

class Bullet {
public:
    Point2 position;
    float speed;

    Bullet(float x, float y) : position(x, y, 0.5f), speed(0.1f) {}  // Smaller size

    void move() {
        position.set(position.getX(), position.getY() + speed);
    }

    void draw() {
        position.draw();
    }
};

class Player {
public:
    Point2 position;
    std::vector<Bullet> bullets;
    float speed;  // Movement speed of the player

    Player(float x, float y, float spd) : position(x, y, 1.0f), speed(spd) {}  // Larger size of bullet

    void shoot() {
        Bullet newBullet(position.getX(), position.getY() + 20.0f);  // Adjust Y position for visibility
        bullets.push_back(newBullet);
        PlaySound(TEXT("bk.wav"), NULL, SND_ASYNC);
    }

    void updateBullets() {
        for (auto &bullet : bullets) {
            bullet.move();
            bullet.draw();
        }
    }

    void moveLeft() {
        position.set(position.getX() - speed, position.getY());
    }

    void moveRight() {
        position.set(position.getX() + speed, position.getY());
    }

    void render() {
        position.draw();
        updateBullets();
    }
};

bool checkCollision(Bullet &bullet, Spider &spider) {
    float bulletX = bullet.position.getX();
    float bulletY = bullet.position.getY();

    float spiderX = spider.spider_getX();
    float spiderY = spider.spider_getY();

    float collisionThreshold = 20.0f;

    float distance = sqrt(pow(bulletX - spiderX, 2) + pow(bulletY - spiderY, 2));

    if (distance < collisionThreshold) {
        spider.die();  // Mark the spider as dead
        return true;   // Collision detected
    }

    return false;
}

Player player(100.0f, 50.0f, 10.0f);  // Example player position
Spider spider(500.0f, 500.0f);  // Example spider position

// Function to render text
void renderGameOverText() {
    glColor3f(1.0, 1.0, 1.0);  // White color
    glRasterPos2f(screenWidth / 2 - 80, screenHeight / 2);  // Position at the center of the screen

    const char *gameOverText = "GAME OVER";
    while (*gameOverText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *gameOverText);  // Use a larger font
        gameOverText++;
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glRasterPos2f(0, 0); // Position to start drawing the background image
    backgroundPixmap.mDraw();

    // Move the spider
    spider.move();

    // Render the spider
    spider.render();

    // Render the player and update bullets
    player.render();

    // Check for bullet-spider collisions
    for (auto &bullet : player.bullets) {
        if (checkCollision(bullet, spider)) {
            break;  // Collision detected
        }
    }

    // Display "GAME OVER" text if the game is over
    if (gameOver) {
        renderGameOverText();
    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    if (!gameOver) {
        switch (key) {
            case 'a': player.moveLeft(); break;  // Move player left
            case 'd': player.moveRight(); break;  // Move player right
            case ' ': player.shoot(); break;  // Shoot bullet
        }
    }
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

    // Initialize background image
    string bgFilename = "Backgroungimage.bmp";  // Replace with your background image file
    backgroundPixmap.readBMPFile(bgFilename, 1);
    backgroundPixmap.setChromaKey(500, 0, 255);  // Set chroma key if necessary
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(screenWidth, screenHeight); // Set to screenWidth and screenHeight
    glutCreateWindow("Spider Shooting Game");

    initOpenGL();

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
