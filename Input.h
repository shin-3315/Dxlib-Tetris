#pragma once

extern bool keyUpPressed;
extern bool keyZPressed;

extern int leftKeyHoldTime;
extern int rightKeyHoldTime;
extern int spaceKeyHoldTime;

extern int fallDelayCounter;
extern const int fallDelayThreshold;

void HandleInput();
void RotateMino(int direction);
void hardDrop();
bool canMoveMino(int dx, int dy);
