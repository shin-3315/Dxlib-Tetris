#include "Dxlib.h"
#include "Mino.h"
#include "Game.h"
#include "Input.h"

int Tmino[4][4] = {
	{0, 1, 0 ,0},
	{1, 1, 1, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0}
};

int Imino[4][4] = {
	{0, 0, 0, 0},
	{1, 1, 1, 1},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
};

int Jmino[4][4] = {
	{0, 1, 0, 0},
	{0, 1, 0, 0},
	{1, 1, 0, 0},
	{0, 0, 0, 0},
};

int Lmino[4][4] = {
	{0, 1, 0, 0},
	{0, 1, 0, 0},
	{0, 1, 1, 0},
	{0, 0, 0, 0}
};

int Zmino[4][4] = {
	{1, 1, 0, 0},
	{0, 1, 1, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0}
};

int Smino[4][4] = {
	{0, 1, 1, 0},
	{1, 1, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0}
};

int Omino[4][4] = {
	{1, 1, 0, 0},
	{1, 1, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0}
};

int wallKickRight_other[4][5][2] = {
	{{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},      //   0° → 90°
	{{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},          //  90°→ 180°
	{{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}},         //  180° → 270°
	{{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}        //  270° → 360°
};

int wallKickLeft_other[4][5][2] = {
	{{0, 0}, {0, 1}, {1, 1}, {0, -2}, {1, -2}},         //   360° → 270°
	{{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},          //  270°→ 180°
	{{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},      //  180° → 90°
	{{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}        //  90° → 0°
};

int wallKickRight_I[4][5][2] = {                       // Iミノバージョン
	{{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}},
	{{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}},
	{{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -1}},
	{{0, 0}, {-2, 0}, {1, 0}, {1, -2}, {-1, 2}},
};

int wallKickLeft_I[4][5][2] = {
	{{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}},
	{{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}},
	{{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}},
	{{0, 0}, {1, 0}, {-2, 0}, {-2, -1}, {1, 2}},
};

int(*currentMino)[4] = nullptr;
int currentColor = 0;
int rotationState = 0;           // 0: 0度, 1: 90度, 2: 180度, 3: 270度

int blockX = 4;  // 初期位置などの設定を行う
int blockY = 0;  // 初期位置などの設定を行う

int ghostY = 0;

int minoBag[7];
int bagIndex = 0;

void ShuffleBag() {
	for (int i = 0; i < 7; ++i) {
		minoBag[i] = i;
	}

	for (int i = 6; i > 0; --i) {
		int j = GetRand(i);
		int temp = minoBag[i];
		minoBag[i] = minoBag[j];
		minoBag[j] = temp;
	}
	bagIndex = 0;
}

void SpawnMino() {
	if (gameOver) return;

	if (bagIndex >= 7) {
		ShuffleBag();
	}

	int nextMinoType = minoBag[bagIndex++];

	switch (nextMinoType) {
	case 0:
		currentMino = Tmino;
		currentColor = GetColor(128, 0, 128);
		break;
	case 1:
		currentMino = Imino;
		currentColor = GetColor(0, 255, 255);
		break;
	case 2:
		currentMino = Jmino;
		currentColor = GetColor(0, 0, 255);
		break;
	case 3:
		currentMino = Lmino;
		currentColor = GetColor(255, 165, 0);
		break;
	case 4:
		currentMino = Zmino;
		currentColor = GetColor(255, 0, 0);
		break;
	case 5:
		currentMino = Smino;
		currentColor = GetColor(0, 255, 0);
		break;
	case 6:
		currentMino = Omino;
		currentColor = GetColor(255, 255, 0);
		break;
	}

	blockX = 4;
	blockY = 0;

	CheckGameOver();
}

void UpdateGhostY() {
	ghostY = blockY;

	while (canMoveMino(0, ghostY - blockY + 1)) {
		ghostY++;
	}
}

