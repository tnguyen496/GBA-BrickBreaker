#include "logic.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "images/sprites.h"
#include "images/yellowBrick.h"
#include "images/redBrick.h"
#include "images/blueBrick.h"



int detectCollisionRect(MovingObj *ball, Brick *brick);
int detectCollision(MovingObj* ball, MovingObj* paddle);

void initializeAppState(AppState* appState) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.

    //Initialize the game state
    appState->score = 0;
    appState->heart = 3;
    appState->gameOver = 0;
    appState->level = 0;
    appState->ballReleased = 1;

    //Initialize the each brick
    int a = 0;
    int b = 0;
    int c = 0;
    for (int i = 0; i < 24; i++) {
        if (i < 8) {
            appState->brick[i].width = 20;
            appState->brick[i].height = 10;
            appState->brick[i].row = 20;
            appState->brick[i].col = 50 + a;
            appState->brick[i].value = 1;
            a = a + 20;
        } else if (i < 16){
            appState->brick[i].width = 20;
            appState->brick[i].height = 10;
            appState->brick[i].row = 30;
            appState->brick[i].col = 50 + b;
            appState->brick[i].value = 1;
            b = b + 20;
        } else {
            appState->brick[i].width = 20;
            appState->brick[i].height = 10;
            appState->brick[i].row = 40;
            appState->brick[i].col = 50 + c;
            appState->brick[i].value = 1;
            c = c + 20;
        }

    }

    //Initialize the paddle
    appState->paddle.cd = 0;
    appState->paddle.rd = 0;
    appState->paddle.width = 30;
    appState->paddle.height = 5;
    appState->paddle.row = 150;
    appState->paddle.col = 108;
    //Initialize the ball
    appState->ball.cd = 1;
    appState->ball.rd = 1;
    appState->ball.width = 5;
    appState->ball.height = 5;
    appState->ball.row = 100;
    appState->ball.col = 120;

}





// TA-TODO: Add any process functions for sub-elements of your app here.
// For example, for a snake game, you could have a processSnake function
// or a createRandomFood function or a processFoods function.
//
// e.g.:
// static Snake processSnake(Snake* currentSnake);
// static void generateRandomFoods(AppState* currentAppState, AppState* nextAppState);

// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    /* TA-TODO: Do all of your app processing here. This function gets called
     * every frame.
     *
     * To check for key presses, use the KEY_JUST_PRESSED macro for cases where
     * you want to detect each key press once, or the KEY_DOWN macro for checking
     * if a button is still down.
     *
     * To count time, suppose that the GameBoy runs at a fixed FPS (60fps) and
     * that VBlank is processed once per frame. Use the vBlankCounter variable
     * and the modulus % operator to do things once every (n) frames. Note that
     * you want to process button every frame regardless (otherwise you will
     * miss inputs.)
     *
     * Do not do any drawing here.
     *
     * TA-TODO: VERY IMPORTANT! READ THIS PART.
     * You need to perform all calculations on the currentAppState passed to you,
     * and perform all state updates on the nextAppState state which we define below
     * and return at the end of the function. YOU SHOULD NOT MODIFY THE CURRENTSTATE.
     * Modifying the currentAppState will mean the undraw function will not be able
     * to undraw it later.
     */

    AppState nextAppState = *currentAppState;

    //Movement of the paddle
    if (KEY_DOWN(BUTTON_RIGHT,keysPressedNow)) {
        if (nextAppState.paddle.col < 208) {
            nextAppState.paddle.col += 4;
        }
    } else if (KEY_DOWN(BUTTON_LEFT,keysPressedNow)) {
        if (nextAppState.paddle.col > 0) {
            nextAppState.paddle.col -= 4;
        }
    } else if (KEY_DOWN(BUTTON_UP,keysPressedNow)) {
        if (nextAppState.paddle.row > 100) {
            nextAppState.paddle.row -= 3;
        }
    } else if (KEY_DOWN(BUTTON_DOWN,keysPressedNow)) {
        if (nextAppState.paddle.row < 154) {
            nextAppState.paddle.row += 3;
        }
    }

    //Releasing the ball & the movements and collisions of the ball
    if(nextAppState.ballReleased == 1)
    {
        if(KEY_JUST_PRESSED(BUTTON_B, keysPressedNow, keysPressedBefore))
        {
            nextAppState.ballReleased = 0;
            nextAppState.ball.row = 100;
            nextAppState.ball.col = 120;
            nextAppState.ball.cd = 1;
            nextAppState.ball.rd = 1;
        }
    } else {

        nextAppState.ball.col += nextAppState.ball.cd;
        nextAppState.ball.row += nextAppState.ball.rd;
        if (nextAppState.ball.row < 0) {
            nextAppState.ball.row = 0;
            nextAppState.ball.rd = -nextAppState.ball.rd;
        }
        if (nextAppState.ball.col < 0) {
            nextAppState.ball.col = 0;
            nextAppState.ball.cd = -nextAppState.ball.cd;
        }
        if (nextAppState.ball.col > 239 - nextAppState.ball.width) {
            nextAppState.ball.col = 239 - nextAppState.ball.width;
            nextAppState.ball.cd = -nextAppState.ball.cd;
        }
        if (nextAppState.ball.row > 159 - nextAppState.ball.height) {
            nextAppState.ball.row = 159 - nextAppState.ball.height;
            nextAppState.ball.rd = -nextAppState.ball.rd;
            nextAppState.heart--;
            nextAppState.ballReleased = 1;
        }

        if (nextAppState.heart == 0) {
            nextAppState.gameOver = 1;
        }

        //detecting collision between the ball and the paddle
        int result = detectCollision(&nextAppState.ball, &nextAppState.paddle);
        if (result) {
            if (result == 2) {
                nextAppState.ball.cd *= -1;
            }
            nextAppState.ball.rd *= -1;
        }

        //detecting the collision between the ball and each brick
        for (int i = 0; i < 24; i++) {
            int result1 = detectCollisionRect(&nextAppState.ball, &nextAppState.brick[i]);
//            int result1 = collision(nextAppState.ball.row, nextAppState.ball.col, nextAppState.ball.height, nextAppState.ball.width,
//                    nextAppState.brick[i].row, nextAppState.brick[i].col, nextAppState.brick[i].height, nextAppState.brick[i].width);
            if (result1) {
                if (result1 == 2 || result1 == 1) {
                    nextAppState.ball.cd *= -1;
                }
                nextAppState.ball.rd *= -1;
                nextAppState.score += 10;
            }
//            if (result1) {
//                nextAppState.brick[i].value = 0;
//                if (result1 == 1) {
//                    nextAppState.ball.rd *= -1;
//                }
//                else if (result1 == 2) {
//                    nextAppState.ball.cd *= -1;
//                }
//            }

        }
    }
    return nextAppState;
}

//function to detect collision between ball and paddle
int detectCollision(MovingObj* ball, MovingObj* paddle)
{
    if(ball->col <= paddle->col + paddle->width &&
       ball->col + ball->width >= paddle->col &&
       ball->row <= paddle->row + paddle->height &&
       ball->height + ball->row >= paddle->row)
    {
        if(ball->col + ball->width == paddle->col)
        {
            return 1;
        }
        else if(ball->col == paddle->col + paddle->width)
        {
            return 2;
        }
        return 3;


    }
    return 0;
}

//function to detect collisions between ball and brick
int detectCollisionRect(MovingObj *ball, Brick *brick)
{
    if(brick->value == 0)
        return 0;
    if(ball->col <= brick->col + brick->width &&
       ball->col + ball->width >= brick->col &&
       ball->row <= brick->row + brick->height &&
       ball->height + ball->row >= brick->row)
    {
        brick->value = 0;
//        right side of ball == left side of brick
        if(ball->col + ball->width == brick->col)
        {
            return 1;
        }
        //left side of ball == right side of brick
        else if(ball->col == brick->col + brick->width)
        {
            return 2;
        }
        return 3;
//        if (ball->row <= brick->row || ball->row > brick->row) {
//            return 1;
//        } else if (ball->col <= brick->col || ball->col > brick->col) {
//            return 2;
//        }

    }
    return 0;
}

int collision(int rowA, int colA, int heightA, int widthA, int rowB, int colB, int heightB, int widthB) {
    return rowA < rowB + heightB - 1 && rowA + heightA - 1 > rowB && colA < colB + widthB - 1 && colA + widthA - 1 > colB;
}


