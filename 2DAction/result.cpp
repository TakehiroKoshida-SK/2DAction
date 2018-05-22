/*
���U���g��ʂ̏������s��ResultModule�N���X�̎���
*/

#include "resultModule.h"

#include "DxLib.h"
#include "module.h"
#include "control.h"
#include "unit.h"
#include <vector>


/*
�R���X�g���N�^
�����l����
*/
ResultModule::ResultModule(int *key) {
	mode = key[1];
	if (mode == 0) playerNum = 2;
	else playerNum = 4;
	for (int i = 0; i < playerNum; i++) {
		controls[i] = Control(i);
		units[i] = Unit(key[2 + i], WariateSide(i), 0, 0);
		runk[i] = key[6 + i];
	}
}

/*
���U���g��ʂ̃��C������
�`��A���͎�t�A���͏����̃��[�v
*/
int* ResultModule::Run() {
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		Draw();
		for (int i = 0; i < playerNum; i++) {
			controls[i].Input();
		}
		if (controls[0].Push(K_X)) {
			nextKey[0] = 0;
			for (int i = 0; i < 6; i++) {
				nextKey[i + 1] = 0;
			}
			for (int i = 0; i < playerNum; i++) {
				nextKey[units[i].myNumber + 1] = (5 - runk[i]);
			}
			return nextKey;
		}
	}
	nextKey[0] = -1;
	return nextKey;
}

/*
���U���g��ʂ̂������`��
*/
void ResultModule::Draw() {
	static int flame = 0;
	flame++;
	//�w�i
	static int resultBackGraph = LoadGraph("graph/other/select_back.jpg");
	DrawExtendGraph(0, 0, 800, 600, resultBackGraph, true);
	//����
	std::string aString = "�肴���";
	SetFontSize(100);
	int x = 400 - GetDrawStringWidth(aString.c_str(), aString.size()) / 2;
	int y = 50;
	DrawString(x, y, aString.c_str(), GetColor(255, 255, 255));
	//���j�b�g�̕`��
	for (int i = 0; i < playerNum; i++) {
		int x = (800 / (playerNum + 1)) * (i + 1);
		int y = 400;
		double rate = 5.5 - runk[i];
		y -= int(32 * rate / 2);
		units[i].DrawAsimoto(x, y, flame / 5, 1.1 * (5.5 - runk[i]));
		units[i].DrawUnit(x, y, controls[i].Hold(K_A), 5.5 - runk[i]);

		int aColor;
		if (units[i].aSide == S_RED) aColor = RED;
		else if (units[i].aSide == S_BLUE) aColor = BLUE;
		else if (units[i].aSide == S_GREEN) aColor = GREEN;
		else aColor = YELLOW;
		SetFontSize(30);
		DrawFormatString(x - 15, 420, aColor, "%dP", i + 1);

	}
	//��������
	aString = "X�{�^���@�Ń^�C�g���ɂ��ǂ�";
	x = 400 - GetDrawStringWidth(aString.c_str(), aString.size()) / 2;
	y = 500;
	DrawString(x, y, aString.c_str(), GetColor(255, 255, 255));


}

/*
��������w�c�����蓖��
*/
side  ResultModule::WariateSide(int num) {
	if (num == 0) return S_RED;
	if (num == 1) {
		if (mode == 2) return S_RED;
		else return S_BLUE;
	}
	if (num == 2) {
		if (mode == 1) return S_GREEN;
		else return S_BLUE;
	}
	else {
		if (mode == 1) return S_YELLOW;
		else return S_BLUE;
	}
}
