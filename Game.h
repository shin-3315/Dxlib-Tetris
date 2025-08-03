#pragma once

// フィールド
extern int field[20][10];
extern int fieldColor[20][10];

// ゲーム状態
extern bool gameOver;
extern int fallCounter;

// リトライボタン関係
extern const int retryX;
extern const int retryY;
extern const int retryWidth;
extern const int retryHeight;

// 関数
void PlaceMino();
void CleanMino();
void CheckGameOver();
void DrawGameOver();
void ResetGame();
void DrawRetryButton();
bool IsRetryClicked();