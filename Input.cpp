#include "DxLib.h"     
#include "Input.h"    
#include "Mino.h"
#include "Game.h" 

bool keyUpPressed = false;
bool keyZPressed = false;

// キー押下時間を管理する変数
int leftKeyHoldTime = 0;
int rightKeyHoldTime = 0;
int spaceKeyHoldTime = 0;

// 下移動のチェック用のカウンター
int fallDelayCounter = 0;
const int fallDelayThreshold = 2;  // ここを大きくすると落下が遅くなる

void HandleInput() {

	// 左移動の処理
	if (CheckHitKey(KEY_INPUT_LEFT)) {
		leftKeyHoldTime++;
		if (leftKeyHoldTime == 1 || leftKeyHoldTime > 10 && leftKeyHoldTime % 2 == 0) {
			for (int y = 0; y < 4; ++y) {
				for (int x = 0; x < 4; ++x) {
					if (currentMino[y][x] == 1 && (blockX + x - 1 < 0 || field[blockY + y][blockX + x - 1] == 1)) {
						return; // 左端を超えるなら移動しない
					}
				}
			}
			blockX--;
		}
	}
	else {
		leftKeyHoldTime = 0;  // キーを離したらリセット
	}

	// 右移動の処理
	if (CheckHitKey(KEY_INPUT_RIGHT)) {
		rightKeyHoldTime++;
		if (rightKeyHoldTime == 1 || rightKeyHoldTime > 10 && rightKeyHoldTime % 2 == 0) {
			for (int y = 0; y < 4; ++y) {
				for (int x = 0; x < 4; ++x) {
					if (currentMino[y][x] == 1 && (blockX + x + 1 >= 10 || field[blockY + y][blockX + x + 1] == 1)) {
						return; // 右端を超えるなら移動しない
					}
				}
			}
			blockX++;
		}
	}
	else {
		rightKeyHoldTime = 0;  // キーを離したらリセット
	}


	// 下移動の処理
	if (CheckHitKey(KEY_INPUT_DOWN)) {
		fallDelayCounter++;
		if (fallDelayCounter >= fallDelayThreshold) {
			for (int y = 0; y < 4; ++y) {
				for (int x = 0; x < 4; ++x) {
					if (currentMino[y][x] == 1 && (blockY + y + 1 >= 20 || field[blockY + y + 1][blockX + x] == 1)) {
						PlaceMino();      //  ミノを固定する 
						SpawnMino();      //  新しいミノを出現させる
						return; // 下端を超えるなら移動しない
					}
				}
			}
			blockY++;
			fallDelayCounter = 0;  // カウンターをリセット
		}
	}
	else {
		fallDelayCounter = 0;  // キーを離したらカウンターリセット
	}

	if (CheckHitKey(KEY_INPUT_UP)) {   // 右回転
		if (!keyUpPressed && currentMino != Omino) {
			RotateMino(1);
			keyUpPressed = true;
		}
	}
	else {
		keyUpPressed = false;
	}

	if (CheckHitKey(KEY_INPUT_Z)) {   // 左回転
		if (!keyZPressed && currentMino != Omino) {
			RotateMino(0);
			keyZPressed = true;
		}
	}
	else {
		keyZPressed = false;
	}

	if (CheckHitKey(KEY_INPUT_SPACE)) {  //ハードドロップをスペースキーで起動
		spaceKeyHoldTime++;
		if (spaceKeyHoldTime == 1) {
			hardDrop();
		}
	}
	else {
		spaceKeyHoldTime = 0;
	}
}

void RotateMino(int direction) {
	int minoSize = (currentMino == Imino) ? 4 : 3;
	int temp[4][4] = { 0 };
	int oldRotation = rotationState;

	// 回転状態を更新
	if (direction == 1) {                // 右回転
		rotationState = (rotationState + 1) % 4;
	}
	else {                             // 左回転
		rotationState = (rotationState + 3) % 4;      // (rotationState - 1 + 4) % 4
	}

	for (int y = 0; y < minoSize; ++y) {
		for (int x = 0; x < minoSize; ++x) {
			if (direction == 1) {
				temp[x][minoSize - 1 - y] = currentMino[y][x];  // 右回転
			}
			else {
				temp[minoSize - 1 - x][y] = currentMino[y][x];  // 左回転
			}
		}
	}
	bool isIMino = (currentMino == Imino);

	int (*kickTable)[5][2] = (direction == 1)
		? (isIMino ? wallKickRight_I : wallKickRight_other)
		: (isIMino ? wallKickLeft_I : wallKickLeft_other);

	for (int i = 0; i < 5; ++i) {
		int offsetX = kickTable[oldRotation][i][0];
		int offsetY = kickTable[oldRotation][i][1];

		bool valid = true;    // 回転可能かどうかのフラグ
		for (int y = 0; y < minoSize; ++y) {
			for (int x = 0; x < minoSize; ++x) {
				if (temp[y][x] == 1) {
					int newX = blockX + x + offsetX;
					int newY = blockY + y + offsetY;

					if (newX < 0 || newY < 0 || newX >= 10 || newY >= 20 || field[newY][newX] == 1) {
						valid = false;
						break;
					}
				}
			}
			if (!valid) break;
		}
		if (valid) {
			// 有効なら回転を確定
			memcpy(currentMino, temp, sizeof(temp));
			blockX += offsetX;
			blockY += offsetY;
			return;
		}
	}
	//壁蹴り失敗なら回転を元に戻す
	rotationState = oldRotation;
}


bool canMoveMino(int dx, int dy) {
	int minoSize = (currentMino == Imino) ? 4 : 3;

	for (int y = 0; y < minoSize; ++y) {
		for (int x = 0; x < minoSize; ++x) {
			if (currentMino[y][x]) {
				int newX = blockX + x + dx;
				int newY = blockY + y + dy;

				if (newX < 0 || newX >= 10 || newY >= 20) {
					return false;
				}

				if (newY >= 0 && field[newY][newX] != 0) {
					return false;
				}
			}
		}
	}
	return true;
}

void hardDrop() {
	while (canMoveMino(0, 1)) {
		blockY++;
	}
	PlaceMino();
}
