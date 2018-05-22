/*
�L�����I����ʂŃv���C���[�ƃJ�[�\���̓����𕪂���₷�����邽�߂̐���UI�̃N���X�̎���
*/

#include "UiLine.h"

#include "DxLib.h"


/*
�R���X�g���N�^
�F�A�w�ԍ��A�������W�A�����J�[�\�����W
*/
UiLine::UiLine(int color, int number, int rootX, int rootY, int baseX, int baseY) {
	this->color = color;
	this->number = number;
	this->rootX = rootX;
	this->rootY = rootY;
	this->baseX = baseX;
	this->baseY = baseY;
	nowX = baseX;
	nowY = baseY;
	frame = -1;
}

/*
�ړ�����Z�b�g����
*/
void UiLine::SetNextPoint(int x, int y) {
	baseX = nowX;
	baseY = nowY;
	toX = x;
	toY = y;
	frame = 0;
}

/*
�����@�t���[�����ƂɌĂт���
*/
void UiLine::Move() {
	if (frame >= 0) {
		nowX = (toX - baseX) * frame / moveTime + baseX;
		nowY = (toY - baseY) * frame / moveTime + baseY;
		frame++;
	}
	if (frame > moveTime) frame = -1;
}

/*
�`��
*/
void UiLine::Draw() {
	static int handle = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawLine(rootX, rootY, nowX, nowY, color);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(nowX - 20, nowY - 14, nowX + 20, nowY + 14, color, false);
	DrawFormatStringToHandle(nowX - 18, nowY - 15, color, handle, "%dP", number + 1);
}