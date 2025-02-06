#include <iostream>
#include<windows.h>
#include<mmsystem.h>
#include<GL/glut.h>
#include<math.h>
#define pi 3.142857
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;
bool freeze = false; // Indicates whether the ball should stop moving

int x = 20;
int y = 20;

int speed = 1;
int high = 300;

int left_limit;
int right_limit;
int up_limit;

bool up = false;
bool move_left = false;
bool move_right = false;

bool reset = false;

int dum = true;

int point = 0;

int box = 7;
int boxx[] = { 150, 400, 700, 500, 900,300, 50 };
int boxy[] = { 90, 160, 230, 300, 370, 440, 510 };
int box_move[] = { 0, 1, 0, 1, 0, 1,0 };

int coin = 7;
int coin_count[] = { 1, 1, 1, 1, 1,1, 1 };

void myInit()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, 1200, 0, 600, -10.0, 10.0);
}

void circle(float x, float y)
{
    float x1, y1, x2, y2;
    float radius = 20;
    x1 = x;
    y1 = y;
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(255, 0, 0);    // red
    glVertex2f(x1, y1);
    float angle;
    for (angle = 0; angle <= 360; angle += 0.5)
    {
        x2 = x1 + sin((angle * 3.1416) / 180) * radius;
        y2 = y1 + cos((angle * 3.1416) / 180) * radius;
        glVertex2f(x2, y2);
    }
    glEnd();
}

struct ScoreEntry {
    string name;
    int score;
};

// Function to load and display leaderboard
void showLeaderboard() {
    ifstream file("scores.txt");
    vector<ScoreEntry> leaderboard;
    ScoreEntry entry;

    while (file >> entry.name >> entry.score) {
        leaderboard.push_back(entry);
    }
    file.close();

    // Sort scores in descending order
    sort(leaderboard.begin(), leaderboard.end(), [](ScoreEntry a, ScoreEntry b) {
        return a.score > b.score;
        });

    cout << "====== LEADERBOARD ======" << endl;
    for (int i = 0; i < min(5, (int)leaderboard.size()); i++) {
        cout << i + 1 << ". " << leaderboard[i].name << " - " << leaderboard[i].score << endl;
    }
}

// Function to save new scores
void saveScore(string playerName, int newScore) {
    ofstream file("scores.txt", ios::app);
    file << playerName << " " << newScore << endl;
    file.close();
}

void circle2(float x, float y)
{
    float x1, y1, x2, y2;
    float radius = 10;
    x1 = x;
    y1 = y;
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(rand() % 255, rand() % 255, rand() % 255);
    glVertex2f(x1, y1);
    float angle;
    for (angle = 0; angle <= 360; angle += 2)
    {
        x2 = x1 + sin((angle * 3.1416) / 180) * radius;
        y2 = y1 + cos((angle * 3.1416) / 180) * radius;
        glVertex2f(x2, y2);
    }
    glEnd();
}

void renderText(float x, float y, const char* text) {
    glRasterPos2f(x, y);  // Set the raster position for text rendering

    // Loop through each character in the text
    for (const char* c = text; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);  // Render each character using GLUT bitmap fonts
    }
}

void rectangle() {

    for (int i = 0;i < box;i++) {
        int xx = boxx[i];
        int yy = boxy[i];

        glBegin(GL_QUADS);  // Begin drawing a quadrilateral
        if (i == 0) glColor3ub(235, 64, 52);
        if (i == 1) glColor3ub(128, 52, 235);
        if (i == 2) glColor3ub(235, 220, 52);
        if (i == 3) glColor3ub(235, 52, 137);
        if (i == 4) glColor3ub(52, 235, 89);
        if (i == 5) glColor3ub(52, 235, 211);
        if (i == 6) glColor3ub(235, 52, 147);

        glVertex2f(xx, yy);  // Define the vertices of the rectangle
        glVertex2f(xx + 140, yy);
        glVertex2f(xx + 140, yy + 20);
        glVertex2f(xx, yy + 20);

        glEnd();
    }
}

void coins() {
    for (int i = 0; i < coin; i++) {
        if (coin_count[i] == 0) continue;  // Skip coins that have been collected

        int xx = boxx[i] + 70;  // Coin's X position (centered on the box)
        int yy = boxy[i] + 30;  // Coin's Y position (above the box)

        // Draw the coin at the calculated position
        circle2(xx, yy);

        // Check if the ball collides with the coin
        double dis = sqrt((xx - x) * (xx - x) + (yy - y) * (yy - y));
        if (dis <= 25.0) {
            coin_count[i] = 0;  // Mark the coin as collected
            point++;            // Increase the score
        }
    }
}


void kata() {
    glBegin(GL_TRIANGLES);  // Begin drawing a quadrilateral

    glColor3ub(rand() % 255, rand() % 255, rand() % 255);

    for (int i = 150;i <= 1170;i += 30) {
        glVertex2f(i, 0);  // Define the vertices of the rectangle
        glVertex2f(i + 15, 20);
        glVertex2f(i + 30, 0);
    }

    glEnd();
}

void text_print() {
    renderText(1000, 580, "Your Total Point Is");

    char numberString[32];
    sprintf(numberString, "%d", point);
    renderText(1100, 560, numberString);
}

void display()
{
    srand(time(0));
    glClear(GL_COLOR_BUFFER_BIT);

    if (reset)
    {
        glColor3ub(255, 0, 0);
        renderText(480, 350, "OPPs!!!    Game Over");
        // Display Final Score with larger size
        char scoreMessage[64];
        sprintf(scoreMessage, "Your Total Point Is: %d", point);
        renderText(490, 300, scoreMessage);
        renderText(500, 250, "Press 'x' to restart");


        glFlush();
        return;
    }

    text_print();
    circle(x, y);        // Draw the ball
    rectangle();         // Draw the moving boxes
    coins();             // Draw and check for coins
    kata();              // Draw static obstacles

    if (x >= 150 && x <= 1170 && y <= 20) reset = true;

    for (int i = 0; i < coin; i++) {
        if (coin_count[i] == 0) continue;

        int xx = boxx[i] + 70;
        int yy = boxy[i] + 30;

        double dis = sqrt((xx - x) * (xx - x) + (yy - y) * (yy - y));
        if (dis <= 25.0) {
            coin_count[i] = 0;
            point++;
        }
    }

    // Update box positions (moving left/right)
    for (int i = 0; i < box; i++) {
        if (box_move[i]) {
            boxx[i] += 1;
            if (boxx[i] == 1050) box_move[i] = 0;
        }
        else {
            boxx[i] -= 1;
            if (boxx[i] == 10) box_move[i] = 1;
        }
    }

    // Vertical movement handling
    if (up) {
        int cnt = 0;

        // Check if the ball is above any moving box
        for (int i = 0; i < box; i++) {
            int xx = boxx[i];
            int yy = boxy[i];

            if (x >= xx && x <= xx + 140 && y == yy - 20) {
                // Ball is above the box, so it moves with the box
                x += box_move[i] == 1 ? 1 : -1; // Moves right or left with the box
                cnt++;
            }
        }

        if (cnt == 0) {
            // Ball moves up normally if not above a box
            y += speed;
            if (y == up_limit) {
                up = false;
            }
        }
        else {
            // If the ball is above a box, it stops moving up but moves with the box
            up = false;
        }
    }

    if (!up) {
        int cnt = 0;

        // Check if the ball is below any moving box
        for (int i = 0; i < box; i++) {
            int xx = boxx[i];
            int yy = boxy[i];

            if (x >= xx && x <= xx + 140 && y == yy + 40) {
                // Ball is below the box, it should move with it
                x += box_move[i] == 1 ? 1 : -1;  // Moves right or left with the box
                cnt++;
                dum = true;
            }
        }

        if (cnt == 0) {
            // Ball moves down normally if not below a box
            y -= speed;
            if (y <= 20) {
                y = 20;
                dum = true;
            }
        }
    }

    // Horizontal movement
    if (move_right) {
        x += 5;
        if (x > right_limit) {
            move_right = false;
        }
    }

    if (move_left) {
        x -= 5;
        if (x < left_limit) {
            move_left = false;
        }
    }

    glFlush();
}

void keyboardFunc(unsigned char key, int a, int b) {
    // Exit the game when 'Esc' or 'q'/'Q' is pressed
    if (key == 27 || key == 'q' || key == 'Q') {
        exit(0);  // Exit the game
    }

    // When the game is in the reset state, and the user presses 'x' or 'X'
    if (reset) {
        if (key == 'x' || key == 'X') {
            // Reset ball position and game variables
            x = 20;
            y = 20;
            up = false;
            move_left = false;
            move_right = false;
            dum = true;
            point = 0;

            // Reset the boxes to their initial positions
            boxx[0] = 150;
            boxx[1] = 400;
            boxx[2] = 700;
            boxx[3] = 500;
            boxx[4] = 900;
            boxx[5] = 300;
            boxx[6] = 50;

            boxy[0] = 100;
            boxy[1] = 200;
            boxy[2] = 250;
            boxy[3] = 320;
            boxy[4] = 370;
            boxy[5] = 470;
            boxy[6] = 520;

            // Reset the box movements and coin counts
            for (int i = 0; i < box; i++) {
                box_move[i] = i % 2; // Assign alternating left/right movements
                coin_count[i] = 1;   // Mark coins as available
            }

            reset = false;  // Clear reset flag
        }
    }
    else {
        // Game controls when it's not in reset state
        if (key == 'd' || key == 'D') {
            // Move the ball to the right
            move_right = true;
            move_left = false;
            right_limit = min(x + 100, 1180);  // Set right boundary for movement
        }
        else if (key == 'a' || key == 'A') {
            // Move the ball to the left
            move_left = true;
            move_right = false;
            left_limit = max(20, x - 100);  // Set left boundary for movement
        }
        else if (key == 'w' || key == 'W') {
            // Make the ball move upward (if not already moving up)
            if (dum) {
                up = true;
                dum = false;
                up_limit = min(y + 200, 600);  // Set upward boundary for movement
            }
        }
        else if (key == 'r' || key == 'R') {
            // Trigger game reset if 'r' is pressed
            reset = true;
        }
    }

    // Ensure the display is updated after the key press
    glutPostRedisplay();
}



void update(int val)
{

    glutPostRedisplay();
    glutTimerFunc(2, update, 0);
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // giving window size in X- and Y- direction
    glutInitWindowSize(1200, 600);
    glutInitWindowPosition(0, 0);

    // Giving name to window 
    glutCreateWindow("Gravity's Grip");
    myInit();

    glutDisplayFunc(display);
    glutTimerFunc(25, update, 0);
    update(0);

    sndPlaySound("sound.wav", SND_ASYNC);
    glutKeyboardFunc(keyboardFunc);
    glutMainLoop();
}
