#include "DxLib.h"
#include "module.h"
#include "titleModule.h"
#include "selectModule.h"
#include "gameModule.h"
#include "resultModule.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// ゲームの起動時の設定
	ChangeWindowMode(FALSE),SetDrawScreen(DX_SCREEN_BACK),DxLib_Init();	// 全画面
	//ChangeWindowMode(TRUE), SetDrawScreen(DX_SCREEN_BACK), DxLib_Init();		// テスト用
	SetGraphMode(800, 600, 32);							// ウィンドウサイズ
	SetDrawScreen(DX_SCREEN_BACK);											// 描画先を裏画面に
	int *key;
	bool finishFlag = false;
	TitleModule aModule;
	key = aModule.Run();
	while (!finishFlag) {
		switch (key[0]) {
		case 0: {TitleModule tModule(key); key = tModule.Run(); } break;
		case 1: {SelectModule sModule(key); key = sModule.Run(); } break;
		case 2: {GameModule gModule(key); key = gModule.Run(); }break;
		case 3: {ResultModule rModule(key); key = rModule.Run(); }break;
		default:finishFlag = true;
		}

	}

	DxLib_End(); // DXライブラリ終了処理
	return 0;
}