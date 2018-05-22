/*
リザルト画面の処理を行うResultModuleクラスの実装
*/

#include "resultModule.h"

#include "DxLib.h"
#include "module.h"
#include "control.h"
#include "unit.h"
#include <vector>


/*
コンストラクタ
初期値いれ
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
リザルト画面のメイン処理
描画、入力受付、入力処理のループ
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
リザルト画面のもろもろ描画
*/
void ResultModule::Draw() {
	static int flame = 0;
	flame++;
	//背景
	static int resultBackGraph = LoadGraph("graph/other/select_back.jpg");
	DrawExtendGraph(0, 0, 800, 600, resultBackGraph, true);
	//文字
	std::string aString = "りざると";
	SetFontSize(100);
	int x = 400 - GetDrawStringWidth(aString.c_str(), aString.size()) / 2;
	int y = 50;
	DrawString(x, y, aString.c_str(), GetColor(255, 255, 255));
	//ユニットの描画
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
	//下部文字
	aString = "Xボタン　でタイトルにもどる";
	x = 400 - GetDrawStringWidth(aString.c_str(), aString.size()) / 2;
	y = 500;
	DrawString(x, y, aString.c_str(), GetColor(255, 255, 255));


}

/*
引数から陣営を割り当て
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
