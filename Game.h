#pragma once

// �t�B�[���h
extern int field[20][10];
extern int fieldColor[20][10];

// �Q�[�����
extern bool gameOver;
extern int fallCounter;

// ���g���C�{�^���֌W
extern const int retryX;
extern const int retryY;
extern const int retryWidth;
extern const int retryHeight;

// �֐�
void PlaceMino();
void CleanMino();
void CheckGameOver();
void DrawGameOver();
void ResetGame();
void DrawRetryButton();
bool IsRetryClicked();