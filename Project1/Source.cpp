#include "DxLib.h"

#define BACKGROUND_PIC_PATH "pic.png"
#define PANEL_WIDTH 80	// �p�l���̕�
#define SQUARE_SIDE 4	// 1�ӂ̃p�l����
#define PANEL_NUM SQUARE_SIDE * SQUARE_SIDE	//�@���p�l����
#define BLANK_PANEL 15

// �O���[�o���ϐ�
int pics[PANEL_NUM];		// PANEL_NUM�����������摜�̃O���t�B�b�N�n���h���̔z��
int original_pic_handle;	// ���摜�̃O���t�B�b�N�n���h��
int panel[PANEL_NUM];		// �p�l���ɕ\�����錳�摜���i�[����z��
enum { TITLE, MAIN, CLEAR } status;

// �I�������p�l���Ƌ󔒂����ւ���
void change(int x, int y) {
	int p1 = y * SQUARE_SIDE + x;
	int p2 = -1;
	if (x > 0 && panel[p1 - 1] == BLANK_PANEL) p2 = p1 - 1;
	if (x < SQUARE_SIDE-1 && panel[p1 + 1] == BLANK_PANEL) p2 = p1 + 1;
	if (y > 0 && panel[p1 - SQUARE_SIDE] == BLANK_PANEL) p2 = p1 - SQUARE_SIDE;
	if (y < SQUARE_SIDE-1 && panel[p1 + SQUARE_SIDE] == BLANK_PANEL) p2 = p1 + SQUARE_SIDE;
	if (p2 != -1) {
		panel[p2] = panel[p1];
		panel[p1] = BLANK_PANEL;
	}
}

// �^�C�g�����
void gameTitle() {
	if (GetMouseInput() & MOUSE_INPUT_LEFT) {
		// �����p�l���̃V���b�t��
		for (int i = 0; i < PANEL_NUM; i++) panel[i] = i;
		for (int i = 0; i < 1000; i++) {
			change(GetRand(SQUARE_SIDE-1), GetRand(SQUARE_SIDE-1));
		}
		status = MAIN;
	}
	DrawGraph(0, 0, original_pic_handle, FALSE);
	DrawString(102, 142, "CLICK TO START", GetColor(255, 0, 0));
}

// ���C�����
void gameMain() {
	if (GetMouseInput() & MOUSE_INPUT_LEFT) {
		int x, y;
		GetMousePoint(&x, &y);
		change(x / PANEL_WIDTH, y / PANEL_WIDTH);
		// �p�l�����S��������������
		bool clear = true;
		for (int i = 0; i < PANEL_NUM; i++) {
			if (panel[i] != i) clear = false;
		}
		if (clear) status = CLEAR;
	}
	// �p�l���̕`��
	for (int i = 0; i < PANEL_NUM; i++) {
		if (panel[i] != BLANK_PANEL) {
			DrawGraph((i % SQUARE_SIDE) * PANEL_WIDTH, (i / SQUARE_SIDE) * PANEL_WIDTH, pics[panel[i]], FALSE);
		}
	}
}

// �I�����
void gameClear() {
	DrawGraph(0, 0, original_pic_handle, FALSE);
	DrawString(115, 142, "GAME CLEAR!", GetColor(255, 0, 0));
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// �t���X�N���[���ɂ��Ȃ�
	ChangeWindowMode(TRUE);
	// ��ʃT�C�Y�̐ݒ�
	SetGraphMode(PANEL_WIDTH * SQUARE_SIDE, PANEL_WIDTH * SQUARE_SIDE, 32);
	// DX���C�u�����̏�����
	DxLib_Init();
	// ��ʂɕ\�������`�揈�����s��
	SetDrawScreen(DX_SCREEN_BACK);

	// �摜��PANEL_NUM�������Ĕz��ɕۑ�
	LoadDivGraph(BACKGROUND_PIC_PATH, PANEL_NUM, SQUARE_SIDE, SQUARE_SIDE, PANEL_WIDTH, PANEL_WIDTH, pics);
	// �������ĂȂ��摜��ǂݍ���
	original_pic_handle = LoadGraph(BACKGROUND_PIC_PATH);

	// �X�e�[�^�X��������(�͂��߂Ƀ^�C�g����ʂ�\��)
	status = TITLE;

	// ���C�����[�v
	while (!ProcessMessage()) {
		ClearDrawScreen();
		switch (status) {
			case TITLE: gameTitle(); break;
			case MAIN: gameMain(); break;
			case CLEAR: gameClear(); break;
		}
		ScreenFlip();
	}
	DxLib_End();
	return 0;
}
