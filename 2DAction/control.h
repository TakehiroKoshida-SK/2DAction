/*
入力関連を処理するControlクラスのヘッダ
*/

#ifndef D2_CONTROL_H_
#define D2_CONTROL_H_

#include "DxLib.h"
#include "define.h"



class Control {
private:
	int num;
	int padKey;
	int input[12];
	int preinp[12];
	int keyReleaseNum[12];
	bool pad;
	int keybordId;
public:
	Control(int num);
	Control();
	void Input();
	bool Push(key aKey);
	bool Hold(key aKey);

	bool Release(key aKey);


};

#endif