#include "DxLib.h"

#define BACKGROUND_PIC_PATH "pic.png"
#define PANEL_WIDTH 80	// パネルの幅
#define SQUARE_SIDE 4	// 1辺のパネル数
#define PANEL_NUM SQUARE_SIDE * SQUARE_SIDE	//　総パネル数
#define BLANK_PANEL 15

// グローバル変数
int pics[PANEL_NUM];		// PANEL_NUM分割した元画像のグラフィックハンドルの配列
int original_pic_handle;	// 元画像のグラフィックハンドル
int panel[PANEL_NUM];		// パネルに表示する元画像を格納する配列
enum { TITLE, MAIN, CLEAR } status;

// 選択したパネルと空白を入れ替える
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

// タイトル画面
void gameTitle() {
	if (GetMouseInput() & MOUSE_INPUT_LEFT) {
		// 初期パネルのシャッフル
		for (int i = 0; i < PANEL_NUM; i++) panel[i] = i;
		for (int i = 0; i < 1000; i++) {
			change(GetRand(SQUARE_SIDE-1), GetRand(SQUARE_SIDE-1));
		}
		status = MAIN;
	}
	DrawGraph(0, 0, original_pic_handle, FALSE);
	DrawString(102, 142, "CLICK TO START", GetColor(255, 0, 0));
}

// メイン画面
void gameMain() {
	if (GetMouseInput() & MOUSE_INPUT_LEFT) {
		int x, y;
		GetMousePoint(&x, &y);
		change(x / PANEL_WIDTH, y / PANEL_WIDTH);
		// パネルが全部揃ったか判定
		bool clear = true;
		for (int i = 0; i < PANEL_NUM; i++) {
			if (panel[i] != i) clear = false;
		}
		if (clear) status = CLEAR;
	}
	// パネルの描画
	for (int i = 0; i < PANEL_NUM; i++) {
		if (panel[i] != BLANK_PANEL) {
			DrawGraph((i % SQUARE_SIDE) * PANEL_WIDTH, (i / SQUARE_SIDE) * PANEL_WIDTH, pics[panel[i]], FALSE);
		}
	}
}

// 終了画面
void gameClear() {
	DrawGraph(0, 0, original_pic_handle, FALSE);
	DrawString(115, 142, "GAME CLEAR!", GetColor(255, 0, 0));
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// フルスクリーンにしない
	ChangeWindowMode(TRUE);
	// 画面サイズの設定
	SetGraphMode(PANEL_WIDTH * SQUARE_SIDE, PANEL_WIDTH * SQUARE_SIDE, 32);
	// DXライブラリの初期化
	DxLib_Init();
	// 画面に表示せず描画処理を行う
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像をPANEL_NUM分割して配列に保存
	LoadDivGraph(BACKGROUND_PIC_PATH, PANEL_NUM, SQUARE_SIDE, SQUARE_SIDE, PANEL_WIDTH, PANEL_WIDTH, pics);
	// 分割してない画像を読み込み
	original_pic_handle = LoadGraph(BACKGROUND_PIC_PATH);

	// ステータスを初期化(はじめにタイトル画面を表示)
	status = TITLE;

	// メインループ
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
