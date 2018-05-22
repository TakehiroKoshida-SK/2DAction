/*
キャラ選択画面の処理をするSelectModuleクラスの実装
*/

#include "selectModule.h"

#include "DxLib.h"
#include "module.h"
#include "control.h"
#include "unit.h"
#include "UiLine.h"

/*
コンストラクタ
モードによって初期値が分岐したり。
*/
SelectModule::SelectModule(int *key) {
	deside = 0;
	mode = key[1];
	for (int i = 0; i < 10; i++) nextKey[i] = 0;
	for (int i = 0; i < 6; i++) {
		unitList[i] = Unit(i, S_RED, 0, 0);
		listFlame[i] = 0;
	}
	for (int i = 0; i < 4; i++) {
		controls[i] = Control(i);
		ok[i] = false;
		side aSide;
		if (mode == 0) {
			if (i == 0) aSide = S_RED;
			else aSide = S_BLUE;
			playerNum = 2;
		}
		else if (mode == 1) {
			if (i == 0) aSide = S_RED;
			else if (i == 1)aSide = S_BLUE;
			else if (i == 2) aSide = S_GREEN;
			else aSide = S_YELLOW;
			playerNum = 4;
		}
		else {
			if (i <= 1) aSide = S_RED;
			else aSide = S_BLUE;
			playerNum = 4;
		}
		sides[i] = aSide;
		selectUnit[i] = Unit(i, aSide, 0, 0);
		selectNum[i] = i;
		int aColor;
		if (sides[i] == S_RED) aColor = RED;
		else if (sides[i] == S_BLUE) aColor = BLUE;
		else if (sides[i] == S_GREEN) aColor = GREEN;
		else aColor = YELLOW;
		lines[i] = UiLine(aColor, i, 100 + i * 200, 100, 75 + (selectNum[i] % 2) * 350, 219 + (selectNum[i] / 2) * 120 + i * 28);
	}
	SetFontSize(30);
	kanryoFont = CreateFontToHandle(NULL, 100, -1);
}

/*
キャラ選択画面のメイン処理
描画、各プレイヤーごとに入力受付、入力処理
全員が選択完了するとアニメーションが入り、それが終わると次のモードへ
*/
int* SelectModule::Run() {
	static point points[4] = { { 0,0 },{ 0,300 },{ 300,0 },{ 300,300 } };
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		int okCount = 0;
		Draw();
		//キャラ選択部分
		if (deside == 0) {	//選択完了アニメーションがinしてない=まだ選択中
			for (int i = 0; i < playerNum; i++) {	//各プレイヤーごとの処理
				controls[i].Input();

				//プレイヤー1のみの操作
				if (i == 0 && controls[i].Push(K_X)) {	//select長押しでタイトルに戻る
					nextKey[0] = 0;
					return nextKey;
				}
				//OKかどうかスイッチ
				if (controls[i].Push(K_A)) ok[i] = true;
				if (controls[i].Push(K_B)) ok[i] = false;
				
				if (ok[i]) okCount++;	//OK状態なら選択済み人数をカウント
				else {	//そうでないなら移動ボタンでキャラを選択
					bool bectleKeyFlag = false;
					if (controls[i].Push(K_UP)) {
						selectNum[i] = (selectNum[i] + 4) % 6;
						bectleKeyFlag = true;
					}
					else if (controls[i].Push(K_RIGHT)) {
						selectNum[i] = selectNum[i] + 1 - (selectNum[i] % 2) * 2;
						bectleKeyFlag = true;
					}
					else if (controls[i].Push(K_DOWN)) {
						selectNum[i] = (selectNum[i] + 2) % 6;
						bectleKeyFlag = true;
					}
					else if (controls[i].Push(K_LEFT)) {
						selectNum[i] = selectNum[i] + 1 - (selectNum[i] % 2) * 2;
						bectleKeyFlag = true;
					}
					//選択ユニットが変わった時の処理
					if (bectleKeyFlag) {
						selectUnit[i] = Unit(selectNum[i], sides[i], 0, 0);
						lines[i].SetNextPoint(75 + (selectNum[i] % 2) * 350, 219 + (selectNum[i] / 2) * 120 + i * 28);	//カーソル移動のアニメーションの登録
					}
				}

				//入力以外の処理
				lines[i].Move();
			}
			if (okCount == playerNum) { deside = 180; }	//全員選択したら選択完了アニメーションをセット
		}
		else {	//選択完了アニメーション
			deside--;
			if (deside == 0) {	//選択完了アニメーション終了　キーを設定して次のモードへ
				nextKey[0] = 2;
				nextKey[1] = mode;
				for (int i = 0; i < playerNum; i++) {
					nextKey[2 + i] = selectNum[i];
				}
				return nextKey;
			}
		}

	}
	//普通ここにはこない
	nextKey[0] = -1;
	return nextKey;
}

/*
キャラ選択画面のもろもろの描画
*/
void SelectModule::Draw() {
	static int flame = 0;
	flame++;

	//背景ペタリ
	static int selectBackGraph = LoadGraph("graph/other/select_back.jpg");
	DrawExtendGraph(0, 0, 800, 600, selectBackGraph, true);

	//キャラ選択肢の描画
	int x, y;
	for (int i = 0; i < 6; i++) {
		x = 50 + (i % 2) * 350;
		y = 200 + (i / 2) * 120;
		//黒背景と枠
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
		DrawBox(x, y, x + 350, y + 120, BLACK, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawBox(x, y, x + 350, y + 120, WHITE, false);
		//ユニット写真とステータス
		unitList[i].DrawUnit(x + 70, y + 60, listFlame[i] / 30);
		DrawString(x + 120, y + 15, "ATK", WHITE);
		for (int j = 0; j < unitList[i].atk; j++) DrawString(x + 180 + j * 30, y + 15, "★", GetColor(220, 255 - j * 60, 0));
		DrawString(x + 120, y + 45, "DEF", WHITE);
		for (int j = 0; j < unitList[i].def; j++) DrawString(x + 180 + j * 30, y + 45, "★", GetColor(220, 255 - j * 60, 0));
		DrawString(x + 120, y + 75, "SPD", WHITE);
		for (int j = 0; j < unitList[i].spd; j++) DrawString(x + 180 + j * 30, y + 75, "★", GetColor(220, 255 - j * 60, 0));

	}

	//プレイヤーの選択状況の描画
	for (int i = 0; i < playerNum; i++) {
		lines[i].Draw();
		x = 100 + i * 200;
		y = 100;
		selectUnit[i].DrawAsimoto(x, y, flame / 5);
		selectUnit[i].DrawUnit(x, y, flame / 30);
		int aColor;
		if (sides[i] == S_RED) aColor = RED;
		else if (sides[i] == S_BLUE) aColor = BLUE;
		else if (sides[i] == S_GREEN) aColor = GREEN;
		else aColor = YELLOW;
		DrawFormatString(x - 70, y - 70, aColor, "%dP", i + 1);
		if (ok[i]) DrawString(x - 20, y + 40, "OK!", aColor);
	}

	//下部のメッセージ
	std::string aString = "Xボタン　でタイトルにもどる";
	x = 400 - GetDrawStringWidth(aString.c_str(), aString.size()) / 2;
	y = 565;
	DrawString(x, y, aString.c_str(), GetColor(255, 255, 255));

	//選択完了アニメーション
	if (deside > 0) {
		if (deside > 160) x = -400 + (180 - deside) * 30;
		else x = 200;
		SetFontSize(100);
		DrawStringToHandle(x, 200, "戦闘開始！", WHITE,kanryoFont);
		SetFontSize(30);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50 + (60 - deside) * 4);
		DrawBox(0, 0, 800, 600, BLACK, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}


}