#include <gui/screen_screen/screenView.hpp>

#include "main.h"
#include <stdio.h>
#include <stdlib.h> //random

#include "texts/TextKeysAndLanguages.hpp"

#include <cstring>

extern int16_t lcounter;
extern int16_t rcounter;

//convert bcd value to decimal of same value
//convert 0x34 to 34 dec.
uint8_t hextodec(uint8_t num) {
	return (10 * ((num & 0xf0) / 16)) + (1 * ((num & 0xf) / 1));
}

#define TFTX 320
#define TFTY 240

#define SPRITEH 40
#define SPRITEW 40

bool gameOver = false;

int xpos = 0;
int ypos = 0;
float zpos = 0.0;

bool Right = true;
bool Down = true;
#define Left !Right
#define Up !Down

int xAccel = 1;
int yAccel = 1;

float zAngle = 0.0;

#define lPaddleHeight 61
#define lPaddleWidth 15

#define rPaddleHeight 61
#define rPaddleWidth 15

int lPaddleXPos = 8;
int lPaddleYPos = 90;
int leftScore = 0;

int rPaddleXPos = 296;
int rPaddleYPos = 90;
int rightScore = 0;

int lppos = 0;
;
int rppos = 0;
;

screenView::screenView() {

}

void screenView::setupScreen() {
	screenViewBase::setupScreen();
	touchgfx::TextureMapper pingu;
	touchgfx::TextureMapper lPaddle;
	touchgfx::TextureMapper rPaddle;

	srand(uwTick); //init random number generator with systick

}

void screenView::tearDownScreen() {
	screenViewBase::tearDownScreen();
}

void screenView::handleTickEvent() {
	tickCounter++;

	Right ? (xpos += xAccel) : (xpos -= xAccel); //move in x direction
	if (!gameOver) {
		//right wall?
		if (xpos > TFTX) {

			leftScore++;
			if (leftScore >= 10) {
				leftScore = 10;
				lWin.setVisible(true);
				lWin.invalidateContent();
				gameOver = true;
			}

			Unicode::snprintf(lScoreBuffer, LSCORE_SIZE, "%d", leftScore);
			lScore.resizeToCurrentText();
			lScore.invalidateContent();

			xpos = 280;
			ypos = 20;

			Right = false;
			Down = true;

		}

		//left wall
		if (xpos < 0 - SPRITEW) {
			if (!gameOver)
				rightScore++;
			if (rightScore >= 10) {
				rightScore = 10;
				rWin.setVisible(true);
				rWin.invalidateContent();
				gameOver = true;
			}

			Unicode::snprintf(rScoreBuffer, RSCORE_SIZE, "%d", rightScore);
			rScore.resizeToCurrentText();
			rScore.invalidateContent();

			xpos = 20;
			ypos = 20;
			Right = true;
			Down = true;
		}
	}

	//check right paddle
	if (Right) {
		int xpos_old = xpos - xAccel;
		if ((xpos_old + SPRITEW <= rPaddleXPos) && (xpos + SPRITEW > rPaddleXPos)) {
			if ((ypos < rppos + rPaddleHeight) && (ypos + SPRITEH > rppos)) {
				Right = false;
				xpos = rPaddleXPos - SPRITEW;
				xAccel = rand() % 5 + 2;

				if (ypos + SPRITEH / 2 < rppos + rPaddleHeight / 3) {
					Down = false; // Hit top part of the paddle, go up
					zAngle = 0.1;
				} else if (ypos + SPRITEH / 2 > rppos + 2 * rPaddleHeight / 3) {
					Down = true; // Hit bottom part, go down
					zAngle = -0.1;
				}
			}
		}
	}
	//check left paddle
	else { // !Right
		int xpos_old = xpos + xAccel;
		if ((xpos_old >= lPaddleXPos + lPaddleWidth) && (xpos < lPaddleXPos + lPaddleWidth)) {
			if ((ypos < lppos + lPaddleHeight) && (ypos + SPRITEH > lppos)) {
				Right = true;
				xpos = lPaddleXPos + lPaddleWidth;
				xAccel = rand() % 5 + 2;

				if (ypos + SPRITEH / 2 < lppos + lPaddleHeight / 3) {
					Down = false;
					zAngle = -0.1;
				} else if (ypos + SPRITEH / 2 > lppos + 2 * lPaddleHeight / 3) {
					Down = true;
					zAngle = 0.1;
				}
			}
		}
	}

	Down ? ypos++ : ypos--;	//move in y direction
	if (ypos > TFTY - SPRITEH) {		//check bottom edge
		Down = false;
		yAccel = rand() % 6;

	}
	if (ypos < 0) {			//check top edge
		Down = true;
		yAccel = rand() % 6;
	}

//    		 		if(Right && Up){
//    		 			zAngle=-0.1;
//    		 		}
//
//    		 		if(Right && Down){
//    		 			zAngle=-0.1;
//    		 		}
//
//
//    		 		if(Left && Up){
//    		 			zAngle=0.1;
//    		 		}
//
//    		 		if(Left && Down){
//    		 			zAngle=0.1;
//    		 		}

//    	pingu.setOrigo(100, 100)
	pingu.updateZAngle(zpos += zAngle);
	pingu.moveTo(xpos, ypos);

	//lPaddle.updateZAngle(1.0); //rotate in radians

	lppos = lPaddleYPos + lcounter * 4;
	lPaddle.moveTo(lPaddleXPos, lppos);

	//rPaddle.updateZAngle(1.5708); //rotate in radians
	rppos = rPaddleYPos + rcounter * 4;
	rPaddle.moveTo(rPaddleXPos, rppos);

	//   }

//    pingu.startMoveAnimation(200, 170, 2, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone);
//    pingu.startMoveAnimation(233, 170, 20, EasingEquations::linearEaseNone, EasingEquations::linearEaseNone);

}

