#include "DxLib.h"
#include "module.h"
#include "titleModule.h"
#include "selectModule.h"
#include "gameModule.h"
#include "resultModule.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// �Q�[���̋N�����̐ݒ�
	ChangeWindowMode(FALSE),SetDrawScreen(DX_SCREEN_BACK),DxLib_Init();	// �S���
	//ChangeWindowMode(TRUE), SetDrawScreen(DX_SCREEN_BACK), DxLib_Init();		// �e�X�g�p
	SetGraphMode(800, 600, 32);							// �E�B���h�E�T�C�Y
	SetDrawScreen(DX_SCREEN_BACK);											// �`���𗠉�ʂ�
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

	DxLib_End(); // DX���C�u�����I������
	return 0;
}