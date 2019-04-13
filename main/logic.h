#ifndef LOGIC_H
#define LOGIC_H

#include "gba.h"


typedef struct {
    int value;
    int row;
    int col;
    int width;
    int height;
}Brick;


typedef struct {
    int row;
    int col;
    int rd;
    int cd;
    int width;
    int height;
} MovingObj;

typedef struct {
    // Store whether or not the game is over in this member:
//    int gameOver;

    /*
    * TA-TODO: Add any logical elements you need to keep track of in your app.
    *
    * For example, for a Snake game, those could be:
    *
    * Snake snake;
    * Food foods[10];
    * int points;
    *
    */

    int score;
    int gameOver;
    int heart;
    MovingObj ball;
    MovingObj paddle;
    Brick brick[24];
    int level;
    int ballReleased;

} AppState;





/*
* TA-TODO: Add any additional structs that you need for your app.
*
* For example, for a Snake game, one could be:
*
* typedef struct {
*   int heading;
*   int length;
*   int x;
*   int y;
* } Snake;
*
*/

//RECT* createLevel(AppState* appState ,short level);

int detectCollision(MovingObj* ball, MovingObj* rect);

int detectCollisionRect(MovingObj *ball, Brick *brick);
int collision(int rowA, int colA, int heightA, int widthA, int rowB, int colB, int heightB, int widthB);
// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);

// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);

// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.

#endif
