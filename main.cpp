#include "DxLib.h"     // DxLib の基本機能（描画、入力など）
#include "Game.h"      // フィールド、ゲームの進行や描画関数
#include "Mino.h"      // ミノの生成や状態
#include "Input.h"     // 入力処理関数（HandleInputなど）

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	int startX = 210;           //  フィールドを中央に寄せるためのX座標
	int startY = 20;            //　フィールドを中央に寄せるためのY座標

	ShuffleBag();       //  最初にシャッフル
	SpawnMino();       // 最初のミノを生成

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {

		if (!gameOver) {
			HandleInput();
			UpdateGhostY();  // ゴーストの位置を更新

			fallCounter++;

			if (fallCounter > 45) {
				bool placed = false;         //ミノの着地判定フラグ（これがないと blockY + 1 のところでミノが生成される）
				for (int y = 0; y < 4; ++y) {
					for (int x = 0; x < 4; ++x) {
						if (currentMino[y][x] == 1 && (blockY + y >= 19 || field[blockY + y + 1][blockX + x])) {
							PlaceMino();
							SpawnMino();
							placed = true;
							break;
						}
					}
				}
				if (!placed) blockY++;

				fallCounter = 0;
			}
		}

		for (int y = 0; y < 20; ++y) {              // フィールドの描画
			for (int x = 0; x < 10; ++x) {
				if (field[y][x] != 0) {
					DrawBox(startX + x * 22, startY + y * 22, startX + (x + 1) * 22, startY + (y + 1) * 22, fieldColor[y][x], TRUE);
					DrawBox(startX + x * 22, startY + y * 22, startX + (x + 1) * 22, startY + (y + 1) * 22, GetColor(255, 255, 255), FALSE);
				}
				else {
					DrawBox(startX + x * 22, startY + y * 22, startX + (x + 1) * 22, startY + (y + 1) * 22, GetColor(50, 50, 50), FALSE);
				}
			}
		}

		for (int y = 0; y < 4; ++y) {
			for (int x = 0; x < 4; ++x) {
				if (currentMino[y][x] == 1) {             // 1ならブロックを描画
					int left = startX + (blockX + x) * 22;
					int top = startY + (blockY + y) * 22;
					int right = startX + (blockX + x + 1) * 22;
					int bottom = startY + (blockY + y + 1) * 22;

					int GhostTop = startY + (ghostY + y) * 22;
					int GhostBottom = startY + (ghostY + y + 1) * 22;

					DrawBox(left, top, right, bottom, currentColor, TRUE);
					DrawBox(left, top, right, bottom, GetColor(255, 255, 255), FALSE);

					// ghostミノの描画（中は灰色で塗りつぶす）
					DrawBox(left, GhostTop, right, GhostBottom, GetColor(180, 180, 180), TRUE);
					// 枠は白で描く
					DrawBox(left, GhostTop, right, GhostBottom, GetColor(255, 255, 255), FALSE);

				}
			}
		}

		if (gameOver) {
			DrawGameOver();
			DrawRetryButton();

			// ボタンが押されたらリセット
			if (IsRetryClicked()) {
				ResetGame();
			}
		}

		WaitTimer(16);
	}

	WaitKey();				// キー入力待ち
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}