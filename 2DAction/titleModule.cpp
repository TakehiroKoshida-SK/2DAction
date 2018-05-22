/*
タイトル画面の処理をするTitleModuleクラスの実装
*/

#include "titleModule.h"

#include "DxLib.h"
#include <string>
#include "module.h"
#include "define.h"
#include "control.h"
#include "unit.h"

//魔に合わせ実装　画面下部のユニットの運動周期　人気キャラほど早く動く
static int circle[6];

/*
コンストラクタ
初期値埋めるだけ
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
タイトル画面でのメイン処理
描画、入力受付、入力処理のループ
*/
int* TitleModule::Run() {
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		Draw();
		aControl.Input();
		flame++;

		//以下、入力処理
		if (aControl.Push(K_UP)) choiceMode = (choiceMode + 2) % 3;
		else if (aControl.Push(K_DOWN)) choiceMode = (choiceMode + 1) % 3;
		else if (aControl.Push(K_RIGHT) || aControl.Push(K_LEFT)) {
			if (choiceMode == 3) choiceMode = 0;
			else choiceMode = 3;
		}

		//終了処理部
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
		//Aボタンが押されたので次のシーンへ　キーを作って終了
		if (aControl.Push(K_A)) {
			if (0 <= choiceMode && choiceMode <= 2) {	//ゲームモードが選択された
				nextKey[0] = 1;
				nextKey[1] = choiceMode;
				return nextKey;
			}
			else {	//終了ボタンが選択された
				nextKey[0] = -1;
				return nextKey;
			}
		}

	}
	//普通ここにはこない
	nextKey[0] = -1;
	return nextKey;

}

/*
タイトル画面のもろもろ描画
*/
 void TitleModule::Draw(){
	 //背景ぺたり
	 static int titleBackGraph = LoadGraph("graph/other/title_back.png");
	 DrawExtendGraph(0, 0, 800, 600, titleBackGraph, true);

	 //選択肢部分の薄黒背景
	 SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	 DrawBox(50, 250, 750, 500, BLACK, true);
	 SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	 //選択肢の文字
	 for (int i = 0; i < 3; i++) {
		 std::string aString;
		 switch (i) {
		 case 0:aString = "２人で　バトルロワイヤル"; break;
		 case 1:aString = "４人で　バトルロワイヤル"; break;
		 case 2:aString = "４人で　チームバトル"; break;
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
	 DrawStringToHandle(575, 465, "ゲームの終了", WHITE, finishFont);
	 if (choiceMode == 3) {
		 DrawBox(565, 460, 745, 495, WHITE, false);
		 DrawBox(567, 462, 743, 493, WHITE, false);
	 }
	 
	 //下部ユニットたち
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