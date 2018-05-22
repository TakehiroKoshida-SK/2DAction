/*
���͊֘A����������Control�N���X�̎���
*/
#include "control.h"

#include "DxLib.h"
#include "define.h"

/*
�R���X�g���N�^
���͂̓R���g���[����ID
�ڑ�����Ă���R���g���[��������Ȃ��ꍇ�̓L�[�{�[�h�Ɋ��蓖�Ă����
*/
Control::Control(int num) {
	this->num = num;
	//printfDx("make Control(%d)\tnow pad num is (%d)\n", num, GetJoypadNum());
	for (int i = 0; i < 12; i++) {
		input[i] = preinp[i] = keyReleaseNum[i] = 0;
	}
	int padNum = GetJoypadNum();
	if (num >= padNum) {
		pad = false;
		keybordId = num - padNum;
	}
	else {
		pad = true;
		switch (num) {
		case 1:padKey = DX_INPUT_PAD2;		break;
		case 2:padKey = DX_INPUT_PAD3;		break;
		case 3:padKey = DX_INPUT_PAD4;		break;
		default:padKey = DX_INPUT_KEY_PAD1;
		}
	}
}
/*
�����Ȃ��̏ꍇ�A�R���g���[��1��p����
*/
Control::Control() {
	padKey = DX_INPUT_KEY_PAD1;
}

/*
���͏���
���t���[������O�ɋN�����Ă���
*/
void Control::Input() {
	for (int i = 0; i < 12; i++) {
		preinp[i] = input[i];
	}

	if (pad == true) {
		int padInp = GetJoypadInputState(padKey);
		input[K_UP] = (padInp & PAD_INPUT_UP);
		input[K_RIGHT] = (padInp & PAD_INPUT_RIGHT);
		input[K_DOWN] = (padInp & PAD_INPUT_DOWN);
		input[K_LEFT] = (padInp & PAD_INPUT_LEFT);
		input[K_A] = (padInp & PAD_INPUT_1);
		input[K_B] = (padInp & PAD_INPUT_2);
		input[K_X] = (padInp & PAD_INPUT_3);
		input[K_Y] = (padInp & PAD_INPUT_4);
		input[K_L] = (padInp & PAD_INPUT_5);
		input[K_R] = (padInp & PAD_INPUT_6);
		input[K_SELECT] = (padInp & PAD_INPUT_7);
		input[K_START] = (padInp & PAD_INPUT_8);
	}
	else {
		if (keybordId == 0) {
			char keyInp[256];
			GetHitKeyStateAll(keyInp);

			input[K_UP] = keyInp[KEY_INPUT_W];
			input[K_RIGHT] = keyInp[KEY_INPUT_D];
			input[K_DOWN] = keyInp[KEY_INPUT_S];
			input[K_LEFT] = keyInp[KEY_INPUT_A];
			input[K_A] = keyInp[KEY_INPUT_SPACE];
			input[K_B] = keyInp[KEY_INPUT_LSHIFT];
			input[K_X] = keyInp[KEY_INPUT_X];
			input[K_Y] = keyInp[KEY_INPUT_Y];
			input[K_L] = keyInp[KEY_INPUT_L];
			input[K_R] = keyInp[KEY_INPUT_R];
			input[K_SELECT] = keyInp[KEY_INPUT_E];
			input[K_START] = keyInp[KEY_INPUT_T];
		}
		if (keybordId == 1) {
			char keyInp[256];
			GetHitKeyStateAll(keyInp);
			input[K_UP] = keyInp[KEY_INPUT_UP];
			input[K_RIGHT] = keyInp[KEY_INPUT_RIGHT];
			input[K_DOWN] = keyInp[KEY_INPUT_DOWN];
			input[K_LEFT] = keyInp[KEY_INPUT_LEFT];
			input[K_A] = keyInp[KEY_INPUT_RETURN];
			input[K_B] = keyInp[KEY_INPUT_RSHIFT];
			input[K_X] = keyInp[KEY_INPUT_1];
			input[K_Y] = keyInp[KEY_INPUT_2];
			input[K_L] = keyInp[KEY_INPUT_3];
			input[K_R] = keyInp[KEY_INPUT_4];
			input[K_SELECT] = keyInp[KEY_INPUT_5];
			input[K_START] = keyInp[KEY_INPUT_6];
		}
		if (keybordId == 2) {
			char keyInp[256];
			GetHitKeyStateAll(keyInp);
			input[K_UP] = keyInp[KEY_INPUT_G];
			input[K_RIGHT] = keyInp[KEY_INPUT_N];
			input[K_DOWN] = keyInp[KEY_INPUT_B];
			input[K_LEFT] = keyInp[KEY_INPUT_V];
			input[K_A] = keyInp[KEY_INPUT_H];
			input[K_B] = keyInp[KEY_INPUT_F];
		}
		if (keybordId == 3) {
			char keyInp[256];
			GetHitKeyStateAll(keyInp);
			input[K_UP] = keyInp[KEY_INPUT_I];
			input[K_RIGHT] = keyInp[KEY_INPUT_L];
			input[K_DOWN] = keyInp[KEY_INPUT_K];
			input[K_LEFT] = keyInp[KEY_INPUT_J];
			input[K_A] = keyInp[KEY_INPUT_O];
			input[K_B] = keyInp[KEY_INPUT_U];
		}

	}
}



/*
�L�[�������ꂽ���ǂ�����Ԃ�
*/
bool Control::Push(key aKey) {
	if (!input[aKey] && preinp[aKey]) return true;	//�O�͉�����Ă��Ȃ��������Ǎ��͉�����Ă���Ȃ�^
	return false;	//������Ă��Ȃ��A�������͒�������ԂȂ̂ŋU
}
/*
�L�[������������Ă��邩��Ԃ�
*/
bool Control::Hold(key aKey) {
	if (input[aKey])	return true;			//������Ă���Ȃ�^
	return false;	//������ĂȂ������畉
}

/*
�L�[�������ꂽ���ǂ�����Ԃ�
*/
bool Control::Release(key aKey) {
	if (!input[aKey] && preinp[aKey]) return true;
	return false;
}