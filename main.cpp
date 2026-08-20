/* ==========================================================
   Project: 2-Player OpenGL Game (Square vs Triangle)
   Course: Computer Graphics / Peripherals & Interfacing

   Students:
   1. MD. Rakibul Islam (ID: 2023-XXXXX)
   2. Mst. Jannatul Ferdous Smrity / Mariya Aktar (Peer Partner)
   ========================================================== */

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

// Player 1: Square coordinates (Blue Chaser)
float xpos1 = 80, ypos1 = 80;
float xpos2 = 80, ypos2 = 40;
float xpos3 = 40, ypos3 = 40;
float xpos4 = 40, ypos4 = 80;

// Player 2: Triangle coordinates (Red Runner)
float tpos1_x = 250, tpos1_y = 350;
float tpos2_x = 220, tpos2_y = 300;
float tpos3_x = 280, tpos3_y = 300;

float deltaX = 4.0f;
float deltaY = 4.0f;

// Rotation angles in degrees
int anti_clockwise = -15;
int clockwise = 15;

// Game state variables
int gameOver = 0;
int winner = 0; // 1 = Square Wins (P1), 2 = Triangle Wins (P2)
int startTime = 0;
int timeLimitSeconds = 180; // 3 minutes time limit

// Helper function to convert degrees to radians
float degreesToRadians(float angle) {
    return angle * (M_PI / 180.0f);
}

// Function to check collisions and boundaries
void checkGameRules() {
    if (gameOver) return;

    // 1. Calculate Square Center
    float sqCenterX = (xpos1 + xpos3) / 2.0f;
    float sqCenterY = (ypos1 + ypos3) / 2.0f;

    // 2. Calculate Triangle Center
    float triCenterX = (tpos1_x + tpos2_x + tpos3_x) / 3.0f;
    float triCenterY = (tpos1_y + tpos2_y + tpos3_y) / 3.0f;

    // 3. Catch Detection
    float dx = sqCenterX - triCenterX;
    float dy = sqCenterY - triCenterY;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance < 25.0f) {
        gameOver = 1;
        winner = 1; // Square caught the triangle -> Player 1 Wins!
        printf("\n[GAME OVER] Player 1 (Square) caught the Triangle! Square Wins!\n");
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Check timer countdown (3 minutes)
    if (!gameOver) {
        int currentTime = glutGet(GLUT_ELAPSED_TIME);
        int elapsedSeconds = (currentTime - startTime) / 1000;

        if (elapsedSeconds >= timeLimitSeconds) {
            gameOver = 1;
            winner = 2; // Time expired without capture -> Player 2 Wins!
            printf("\n[GAME OVER] Time's up! Player 2 (Triangle) survived! Triangle Wins!\n");
        }
    }

    // Render Player 1: Blue Square
    glColor3f(0.0f, 0.6f, 1.0f);
    glBegin(GL_POLYGON);
        glVertex2f(xpos1, ypos1);
        glVertex2f(xpos2, ypos2);
        glVertex2f(xpos3, ypos3);
        glVertex2f(xpos4, ypos4);
    glEnd();

    // Render Player 2: Red Triangle
    glColor3f(1.0f, 0.2f, 0.3f);
    glBegin(GL_TRIANGLES);
        glVertex2f(tpos1_x, tpos1_y);
        glVertex2f(tpos2_x, tpos2_y);
        glVertex2f(tpos3_x, tpos3_y);
    glEnd();

    glutSwapBuffers();
}

void customization() {
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f); // Dark tactical background
    startTime = glutGet(GLUT_ELAPSED_TIME);
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
    glMatrixMode(GL_MODELVIEW);
}

void timerCallback(int value) {
    if (!gameOver) {
        checkGameRules();
        glutPostRedisplay();
        glutTimerFunc(16, timerCallback, 0); // ~60 FPS frame refresh loop
    }
}

void keyboardfunc(unsigned char key, int x, int y) {
    if (gameOver) return;

    switch (key) {
        case 'w': case 'W':
            ypos1 += deltaY; ypos2 += deltaY;
            ypos3 += deltaY; ypos4 += deltaY;
            break;
        case 's': case 'S':
            ypos1 -= deltaY; ypos2 -= deltaY;
            ypos3 -= deltaY; ypos4 -= deltaY;
            break;
        case 'a': case 'A':
            xpos1 -= deltaX; xpos2 -= deltaX;
            xpos3 -= deltaX; xpos4 -= deltaX;
            break;
        case 'd': case 'D':
            xpos1 += deltaX; xpos2 += deltaX;
            xpos3 += deltaX; xpos4 += deltaX;
            break;
        case 'f': case 'F': { // Rotate anti-clockwise around center
            float centerX = (xpos1 + xpos3) / 2.0f;
            float centerY = (ypos1 + ypos3) / 2.0f;
            float angle = degreesToRadians(anti_clockwise);
            float temp;

            temp = xpos1 - centerX;
            xpos1 = centerX + (temp * cos(angle) - (ypos1 - centerY) * sin(angle));
            ypos1 = centerY + (temp * sin(angle) + (ypos1 - centerY) * cos(angle));

            temp = xpos2 - centerX;
            xpos2 = centerX + (temp * cos(angle) - (ypos2 - centerY) * sin(angle));
            ypos2 = centerY + (temp * sin(angle) + (ypos2 - centerY) * cos(angle));

            temp = xpos3 - centerX;
            xpos3 = centerX + (temp * cos(angle) - (ypos3 - centerY) * sin(angle));
            ypos3 = centerY + (temp * sin(angle) + (ypos3 - centerY) * cos(angle));

            temp = xpos4 - centerX;
            xpos4 = centerX + (temp * cos(angle) - (ypos4 - centerY) * sin(angle));
            ypos4 = centerY + (temp * sin(angle) + (ypos4 - centerY) * cos(angle));
            break;
        }
        case 'g': case 'G': { // Rotate clockwise around center
            float centerX = (xpos1 + xpos3) / 2.0f;
            float centerY = (ypos1 + ypos3) / 2.0f;
            float angle = degreesToRadians(clockwise);
            float temp;

            temp = xpos1 - centerX;
            xpos1 = centerX + (temp * cos(angle) - (ypos1 - centerY) * sin(angle));
            ypos1 = centerY + (temp * sin(angle) + (ypos1 - centerY) * cos(angle));

            temp = xpos2 - centerX;
            xpos2 = centerX + (temp * cos(angle) - (ypos2 - centerY) * sin(angle));
            ypos2 = centerY + (temp * sin(angle) + (ypos2 - centerY) * cos(angle));

            temp = xpos3 - centerX;
            xpos3 = centerX + (temp * cos(angle) - (ypos3 - centerY) * sin(angle));
            ypos3 = centerY + (temp * sin(angle) + (ypos3 - centerY) * cos(angle));

            temp = xpos4 - centerX;
            xpos4 = centerX + (temp * cos(angle) - (ypos4 - centerY) * sin(angle));
            ypos4 = centerY + (temp * sin(angle) + (ypos4 - centerY) * cos(angle));
            break;
        }
    }
    checkGameRules();
    glutPostRedisplay();
}

void onMouseClick(int button, int state_click, int x, int y) {
    if (gameOver) return;

    if (button == GLUT_LEFT_BUTTON && state_click == GLUT_UP) {
        int oglY = 500 - y; // OpenGL bottom-left coordinate mapping

        // Player 2 repositioning via mouse clicks
        tpos1_x = x;       tpos1_y = oglY + 18;
        tpos2_x = x - 18;  tpos2_y = oglY - 12;
        tpos3_x = x + 18;  tpos3_y = oglY - 12;

        checkGameRules();
        glutPostRedisplay();
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowPosition(100, 50);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Square vs Triangle - 2 Player Tactical Game");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardfunc);
    glutMouseFunc(onMouseClick);

    customization();
    glutTimerFunc(0, timerCallback, 0);
    glutMainLoop();
    return 0;
}
