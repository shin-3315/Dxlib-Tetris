#pragma once

// �~�m�̌`
extern int Tmino[4][4];
extern int Imino[4][4];
extern int Jmino[4][4];
extern int Lmino[4][4];
extern int Zmino[4][4];
extern int Smino[4][4];
extern int Omino[4][4];

// �ǃL�b�N�̃f�[�^�i�E��]�E����]�j
extern int wallKickRight_other[4][5][2];
extern int wallKickLeft_other[4][5][2];
extern int wallKickRight_I[4][5][2];
extern int wallKickLeft_I[4][5][2];

// ���݂̃~�m���
extern int (*currentMino)[4];
extern int currentColor;
extern int rotationState;
extern int blockX;
extern int blockY;
extern int ghostY;

// �~�m�V���b�t���i7�o�b�O�j
extern int minoBag[7];
extern int bagIndex;

// �֐�
void ShuffleBag();
void SpawnMino();
void UpdateGhostY();