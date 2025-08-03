#include "DxLib.h"
#include "Game.h"
#include "Mino.h"

int field[20][10] = { 0 };
int fieldColor[20][10] = { 0 };

bool gameOver = false;
int fallCounter = 0;

const int retryX = 480;
const int retryY = 400;
const int retryWidth = 120;
const int retryHeight = 40;

void PlaceMino() {
	for (int y = 0; y < 4; ++y) {
		for (int x = 0; x < 4; ++x) {
			if (currentMino[y][x] == 1) {
				int fieldX = blockX + x;
				int fieldY = blockY + y;

				// フィールドの範囲内にあるか確認　（バグ時のクラッシュ対策）
				if (fieldX >= 0 && fieldX < 10 && fieldY >= 0 && fieldY < 20) {
					field[fieldY][fieldX] = 1;
					fieldColor[fieldY][fieldX] = currentColor;
				}
			}
		}
	}
	CleanMino();
}

void CleanMino() {
	for (int y = 0; y < 20; ++y) {
		bool isFullLine = true;      // この行が埋まっているか判定するフラグ

		for (int x = 0; x < 10; ++x) {
			if (field[y][x] == 0) {
				isFullLine = false;
				break;       // 一つでも空きがあれば行はそろっていないので次の行へ
			}
		}
		// もしラインが揃っていたら削除＆上のブロックを落とす
		if (isFullLine) {
			for (int newY = y; newY > 0; --newY) {
				for (int x = 0; x < 10; ++x) {
					field[newY][x] = field[newY - 1][x];
					fieldColor[newY][x] = fieldColor[newY - 1][x];
				}
			}
		}
	}
}

// ゲームオーバー処理
void CheckGameOver() {
	// 新しいミノが生成される前に、フィールドの上端にすでにミノがあるかどうかをチェック
	for (int x = 0; x < 4; ++x) {
		if (currentMino[0][x] == 1 && field[blockY][blockX + x] != 0) {
			gameOver = true;
			break;
		}
	}
}
// ゲームオーバー表示
void DrawGameOver() {
	DrawString(80, 200, "Game Over", GetColor(255, 0, 0));  // 赤い文字で表示
}

// ゲーム初期化
void ResetGame() {
	for (int y = 0; y < 20; ++y)
		for (int x = 0; x < 10; ++x) {
			field[y][x] = 0;
			fieldColor[y][x] = 0;
		}

	gameOver = false;
	bagIndex = 7;
	fallCounter = 0;
	ShuffleBag();
	SpawnMino();
}

// リトライボタンの描画
void DrawRetryButton() {
	DrawBox(retryX, retryY, retryX + retryWidth, retryY + retryHeight, GetColor(200, 200, 200), TRUE);
	DrawBox(retryX, retryY, retryX + retryWidth, retryY + retryHeight, GetColor(0, 0, 0), FALSE);
	DrawString(retryX + 40, retryY + 10, "Retry", GetColor(0, 0, 0));
}

// リトライボタンが押されたかを判定
bool IsRetryClicked() {
	static int oldInput = 0;

	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	int currentInput = GetMouseInput();

	bool isInside = (mouseX >= retryX && mouseX <= retryX + retryWidth &&
		mouseY >= retryY && mouseY <= retryY + retryHeight);

	bool isClicked = ((currentInput & MOUSE_INPUT_LEFT) && !(oldInput & MOUSE_INPUT_LEFT));

	oldInput = currentInput;

	return (isInside && isClicked);
}