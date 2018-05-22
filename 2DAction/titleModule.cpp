/*
�^�C�g����ʂ̏���������TitleModule�N���X�̎���
*/

#include "titleModule.h"

#include "DxLib.h"
#include <string>
#include "module.h"
#include "define.h"
#include "control.h"
#include "unit.h"

//���ɍ��킹�����@��ʉ����̃��j�b�g�̉^�������@�l�C�L�����قǑ�������
static int circle[6];

/*
�R���X�g���N�^
�����l���߂邾��
*/
TitleModule::TitleModule(){
	aControl = Control(0);
	flame = 0;
	choiceMode = 0;
	SetFontSize(35);
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);
	for (int i = 0; i < 6; i++) {
		units[i] = Unit(i, S_RED, 0, 0);
		move[i] = 0;
		circle[i] = 180;
	}
}
TitleModule::TitleModule(int *key) {
	aControl = Control(0);
	flame = 0;
	choiceMode = 0;
	SetFontSize(35);
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);
	for (int i = 0; i < 6; i++) {
		units[i] = Unit(i, S_RED, 0, 0);
		move[i] = 0;
		circle[i] = circle[i] * 15 / (15 + key[i + 1]);
		if (circle[i] < 1) circle[i] = 1;
	}
}

/*
�^�C�g����ʂł̃��C������
�`��A���͎�t�A���͏����̃��[�v
*/
int* TitleModule::Run() {
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		Draw();
		aControl.Input();
		flame++;

		//�ȉ��A���͏���
		if (aControl.Push(K_UP)) choiceMode = (choiceMode + 2) % 3;
		else if (aControl.Push(K_DOWN)) choiceMode = (choiceMode + 1) % 3;
		else if (aControl.Push(K_RIGHT) || aControl.Push(K_LEFT)) {
			if (choiceMode == 3) choiceMode = 0;
			else choiceMode = 3;
		}

		//�I��������
		static int selectCount = 0;	
		if (aControl.Hold(K_SELECT)) {
			selectCount++;
			if (selectCount > 60) {
				selectCount = 0;
				nextKey[0] = -1;
				return nextKey;
			}
		}
		else selectCount = 0;
		//A�{�^���������ꂽ�̂Ŏ��̃V�[���ց@�L�[������ďI��
		if (aControl.Push(K_A)) {
			if (0 <= choiceMode && choiceMode <= 2) {	//�Q�[�����[�h���I�����ꂽ
				nextKey[0] = 1;
				nextKey[1] = choiceMode;
				return nextKey;
			}
			else {	//�I���{�^�����I�����ꂽ
				nextKey[0] = -1;
				return nextKey;
			}
		}

	}
	//���ʂ����ɂ͂��Ȃ�
	nextKey[0] = -1;
	return nextKey;

}

/*
�^�C�g����ʂ̂������`��
*/
 void TitleModule::Draw(){
	 //�w�i�؂���
	 static int titleBackGraph = LoadGraph("graph/other/title_back.png");
	 DrawExtendGraph(0, 0, 800, 600, titleBackGraph, true);

	 //�I���������̔����w�i
	 SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	 DrawBox(50, 250, 750, 500, BLACK, true);
	 SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	 //�I�����̕���
	 for (int i = 0; i < 3; i++) {
		 std::string aString;
		 switch (i) {
		 case 0:aString = "�Q�l�Ł@�o�g�������C����"; break;
		 case 1:aString = "�S�l�Ł@�o�g�������C����"; break;
		 case 2:aString = "�S�l�Ł@�`�[���o�g��"; break;
		 }
		 /*
		 int x = 350 - GetDrawStringWidth(aString.c_str(), aString.size()) / 2;
		 int y = 270 + i * 70;
		 DrawString(x, y, aString.c_str(), WHITE);
		 if (choiceMode == i) {
			 DrawBox(90, y - 10, 610, y + 50, WHITE, false);
			 DrawBox(92, y - 8, 608, y + 48, WHITE, false);
		 }
		 */
		 int x = 400 - GetDrawStringWidth(aString.c_str(), aString.size()) / 2;
		 int y = 270 + i * 70;
		 DrawString(x, y, aString.c_str(), WHITE);
		 if (choiceMode == i) {
			 DrawBox(140, y - 10, 660, y + 50, WHITE, false);
			 DrawBox(142, y - 8, 658, y + 48, WHITE, false);
		 }

	 }

	 static int finishFont = CreateFontToHandle(NULL, 25, -1);
	 DrawStringToHandle(575, 465, "�Q�[���̏I��", WHITE, finishFont);
	 if (choiceMode == 3) {
		 DrawBox(565, 460, 745, 495, WHITE, false);
		 DrawBox(567, 462, 743, 493, WHITE, false);
	 }
	 
	 //�������j�b�g����
	 for (int i = 0; i < 6; i++) {
		 int num;
		 switch (i) {
		 case 0: num = 2; break;
		 case 1: num = 3; break;
		 case 2: num = 1; break;
		 case 3: num = 4; break;
		 case 4: num = 0; break;
		 default: num = 5; break;
		 }
		 units[i].DrawUnit(67 + num * 800 / 6, 550, move[i], 3);
		 if (flame % circle[i] == 0) move[i]++;
	 }

 }