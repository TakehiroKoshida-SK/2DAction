/*
�L�����I����ʂŃv���C���[�ƃJ�[�\���̓����𕪂���₷�����邽�߂̐���UI�̃N���X�̃w�b�_
*/



#ifndef D2_UiLine_H_
#define D2_UiLine_H_

#include "DxLib.h"

const int moveTime = 8;

class UiLine {
private:
	int color;
	int number;
	int rootX;
	int rootY;
	int frame;
	int baseX;
	int baseY;
	int toX;
	int toY;
	int nowX;
	int nowY;
public:
	UiLine() { ; }
	UiLine(int color, int number, int rootX, int rootY, int baseX, int baseY);
	void SetNextPoint(int x, int y);
	void Move();
	void Draw();
};

#endif