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

				// �t�B�[���h�͈͓̔��ɂ��邩�m�F�@�i�o�O���̃N���b�V���΍�j
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
		bool isFullLine = true;      // ���̍s�����܂��Ă��邩���肷��t���O

		for (int x = 0; x < 10; ++x) {
			if (field[y][x] == 0) {
				isFullLine = false;
				break;       // ��ł��󂫂�����΍s�͂�����Ă��Ȃ��̂Ŏ��̍s��
			}
		}
		// �������C���������Ă�����폜����̃u���b�N�𗎂Ƃ�
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

// �Q�[���I�[�o�[����
void CheckGameOver() {
	// �V�����~�m�����������O�ɁA�t�B�[���h�̏�[�ɂ��łɃ~�m�����邩�ǂ������`�F�b�N
	for (int x = 0; x < 4; ++x) {
		if (currentMino[0][x] == 1 && field[blockY][blockX + x] != 0) {
			gameOver = true;
			break;
		}
	}
}
// �Q�[���I�[�o�[�\��
void DrawGameOver() {
	DrawString(80, 200, "Game Over", GetColor(255, 0, 0));  // �Ԃ������ŕ\��
}

// �Q�[��������
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

// ���g���C�{�^���̕`��
void DrawRetryButton() {
	DrawBox(retryX, retryY, retryX + retryWidth, retryY + retryHeight, GetColor(200, 200, 200), TRUE);
	DrawBox(retryX, retryY, retryX + retryWidth, retryY + retryHeight, GetColor(0, 0, 0), FALSE);
	DrawString(retryX + 40, retryY + 10, "Retry", GetColor(0, 0, 0));
}

// ���g���C�{�^���������ꂽ���𔻒�
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