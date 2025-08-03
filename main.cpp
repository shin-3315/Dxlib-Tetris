#include "DxLib.h"     // DxLib �̊�{�@�\�i�`��A���͂Ȃǁj
#include "Game.h"      // �t�B�[���h�A�Q�[���̐i�s��`��֐�
#include "Mino.h"      // �~�m�̐�������
#include "Input.h"     // ���͏����֐��iHandleInput�Ȃǁj

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	int startX = 210;           //  �t�B�[���h�𒆉��Ɋ񂹂邽�߂�X���W
	int startY = 20;            //�@�t�B�[���h�𒆉��Ɋ񂹂邽�߂�Y���W

	ShuffleBag();       //  �ŏ��ɃV���b�t��
	SpawnMino();       // �ŏ��̃~�m�𐶐�

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {

		if (!gameOver) {
			HandleInput();
			UpdateGhostY();  // �S�[�X�g�̈ʒu���X�V

			fallCounter++;

			if (fallCounter > 45) {
				bool placed = false;         //�~�m�̒��n����t���O�i���ꂪ�Ȃ��� blockY + 1 �̂Ƃ���Ń~�m�����������j
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

		for (int y = 0; y < 20; ++y) {              // �t�B�[���h�̕`��
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
				if (currentMino[y][x] == 1) {             // 1�Ȃ�u���b�N��`��
					int left = startX + (blockX + x) * 22;
					int top = startY + (blockY + y) * 22;
					int right = startX + (blockX + x + 1) * 22;
					int bottom = startY + (blockY + y + 1) * 22;

					int GhostTop = startY + (ghostY + y) * 22;
					int GhostBottom = startY + (ghostY + y + 1) * 22;

					DrawBox(left, top, right, bottom, currentColor, TRUE);
					DrawBox(left, top, right, bottom, GetColor(255, 255, 255), FALSE);

					// ghost�~�m�̕`��i���͊D�F�œh��Ԃ��j
					DrawBox(left, GhostTop, right, GhostBottom, GetColor(180, 180, 180), TRUE);
					// �g�͔��ŕ`��
					DrawBox(left, GhostTop, right, GhostBottom, GetColor(255, 255, 255), FALSE);

				}
			}
		}

		if (gameOver) {
			DrawGameOver();
			DrawRetryButton();

			// �{�^���������ꂽ�烊�Z�b�g
			if (IsRetryClicked()) {
				ResetGame();
			}
		}

		WaitTimer(16);
	}

	WaitKey();				// �L�[���͑҂�
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}