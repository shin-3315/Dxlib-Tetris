#pragma once

// ミノの形
extern int Tmino[4][4];
extern int Imino[4][4];
extern int Jmino[4][4];
extern int Lmino[4][4];
extern int Zmino[4][4];
extern int Smino[4][4];
extern int Omino[4][4];

// 壁キックのデータ（右回転・左回転）
extern int wallKickRight_other[4][5][2];
extern int wallKickLeft_other[4][5][2];
extern int wallKickRight_I[4][5][2];
extern int wallKickLeft_I[4][5][2];

// 現在のミノ状態
extern int (*currentMino)[4];
extern int currentColor;
extern int rotationState;
extern int blockX;
extern int blockY;
extern int ghostY;

// ミノシャッフル（7バッグ）
extern int minoBag[7];
extern int bagIndex;

// 関数
void ShuffleBag();
void SpawnMino();
void UpdateGhostY();