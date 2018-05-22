/*
ゲーム画面の処理を行うGameModuleクラスの実装
*/

#include "gameModule.h"

#include "DxLib.h"
#include "module.h"
#include "control.h"
#include "unit.h"
#include "map.h"
#include <math.h>

/*
コンストラクタ
初期値いれ
*/
GameModule::GameModule(int *key) {
	count = 0;
	countdown = 0;
	sokomade = -1;
	deadNum = 0;
	mode = key[1];
	for (int i = 0; i < 10; i++) nextKey[i] = 0;
	if (mode == 0) playerNum = 2;
	else playerNum = 4;
	for (int i = 0; i < playerNum; i++) {
		controls[i] = Control(i);
	}
	map = Map(GetRand(3) + 1, mode);
	//マップにユニットを立たせる処理
	for (int x = 0; x < 20; x++) {
		for (int y = 0; y < 13; y++) {
			if (map.mapData[x][y].on >= 0) {
				int on = map.mapData[x][y].on;
				Unit aUnit(key[2 + on], WariateSide(on), x, y);
				if (playerNum == 2) aUnit.maxHp = aUnit.hitPoint = aUnit.maxHp * 2 / 3;
				if (mode == 2) {
					if (aUnit.aSide == S_RED) aUnit.MemoryEnemyPoint(map.bluePoint);
					else if (aUnit.aSide == S_BLUE) aUnit.MemoryEnemyPoint(map.redPoint);
				}
				units[on] = aUnit;
			}
		}
	}
	damageFont = CreateFontToHandle(NULL, 20, -1, DX_FONTTYPE_EDGE);
	animeFont = CreateFontToHandle(NULL, 100, -1);
}

/*
ゲーム画面のメイン処理
開始アニメーション、ゲーム処理、終了アニメーションに分岐
*/
int* GameModule::Run(){
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		count++;
		Draw();
		//開始アニメーション
		if (countdown >= 0) {
			for (int i = 0; i < 5; i++) {
				int x = 400;
				int y = -100;;
				int teisu = i * 60;
				if (teisu <= countdown && countdown < teisu + 10) y = 30 * (countdown - teisu) - 100;
				if (teisu + 10 <= countdown && countdown < teisu + 60) y = 200;
				if (teisu + 60 <= countdown && countdown < teisu + 80) y = 200 + 30 * (countdown - teisu - 60);
				if (i == 0) DrawStringToHandle(x - 100, y, "RADY?", GetColor(255, 255, 255),animeFont);
				else if (i < 4) DrawFormatStringToHandle(x - 25, y, GetColor(255, 255, 255),animeFont, "%d", 4 - i);
				else DrawStringToHandle(x - 100, y, "ＧＯ！", GetColor(255, 255, 255),animeFont);
			}
			countdown++;
			if (countdown >= 280) countdown = -1;

		}

		//ゲーム処理
		else if (sokomade == -1) {
			AtackCur();
			itemCur();
			static int selectCount = 0;
			if (controls[0].Hold(K_SELECT)) {
				selectCount++;
				if (selectCount > 60) {
					selectCount = 0;
					nextKey[0] = 0;
					return nextKey;
				}
			}
			else selectCount = 0;
			for (int i = 0; i < playerNum; i++) {


				controls[i].Input();
				//移動処理
				int x = units[i].x;
				int y = units[i].y;
				if (units[i].Movable() == 2) {

					int bectleInput = -1;
					if (controls[i].Hold(K_UP)) bectleInput = 0;
					else if (controls[i].Hold(K_RIGHT)) bectleInput = 1;
					else if (controls[i].Hold(K_DOWN)) bectleInput = 2;
					else if (controls[i].Hold(K_LEFT)) bectleInput = 3;
					if (bectleInput != -1) {
						mData aData = map.mapData[x + urdlX[bectleInput]][y + urdlY[bectleInput]];
						//ステップ移動　向きを維持して移動できるが、硬直大き目
						if (controls[i].Hold(K_B) && aData.aArea != A_WALL && aData.Hp <= 0 && aData.on == -1) {
							units[i].x = x + urdlX[bectleInput];
							units[i].y = y + urdlY[bectleInput];
							map.mapData[x][y].on = -1;
							map.mapData[x + urdlX[bectleInput]][y + urdlY[bectleInput]].on = i;
							units[i].idoukouchoku += units[i].idoukouchoku = 15 / units[i].spd + 10;
							units[i].muteki += 5 + units[i].spd;
						}
						//方向転換
						else if (units[i].bectle != bectleInput) {
							units[i].bectle = bectleInput;
							units[i].idoukouchoku = 5;
						}
						//通常の移動。　チャージ中は移動が遅くなる。
						else if (aData.aArea != A_WALL && aData.Hp <= 0 && aData.on == -1) {
							units[i].x = x + urdlX[bectleInput];
							units[i].y = y + urdlY[bectleInput];
							map.mapData[x][y].on = -1;
							map.mapData[x + urdlX[bectleInput]][y + urdlY[bectleInput]].on = i;
							if (units[i].charge <= 0)	units[i].idoukouchoku = 21 / units[i].spd + 3;
							else					units[i].idoukouchoku = 21 / units[i].spd + 13;
						}
					}
				}
				//攻撃処理
				if (units[i].Movable() >= 1) {

					if (controls[i].Release(K_A)) {
						int num = units[i].chargeList.size();
						int frontLag = 16 / units[i].spd + 5;
						effect aEffect = { units[i].myNumber,-frontLag,x,y,units[i].bectle };
						//	チャージ攻撃の処理
						if (units[i].charge >= units[i].GetChargeTime()) {
							aEffect.type += 10;
							//突進以外の攻撃処理。　チャージリストの基礎データに追加情報を与えて処理配列に追加
							if (units[i].chargeList[0].tosshin == 0) {
								for (int j = 0; j < num; j++) {
									attack aAtack = units[i].chargeList[j];
									aAtack = SettingAttack(aAtack, i);
									if (aAtack.x < 0 || 20 <= aAtack.x || aAtack.y < 0 || 13 <= aAtack.y) { break; }
									attackTask.push_back(aAtack);
								}
							}
							//突進攻撃の処理。　ユニットを突進モードに。
							else {
								//units[i].tossin = units[i].chargeList[0].tosshin;

								std::vector<attack> tmpList(TosshinCur(units[i].chargeList, i));
								for (int j = 0; j < num; j++) {
									attack aAtack = tmpList[j];
									if (aAtack.aType == T_MAIN) {
										map.mapData[units[i].x][units[i].y].on = -1;
										units[i].x = aAtack.x - urdlX[units[i].bectle];
										units[i].y = aAtack.y - urdlY[units[i].bectle];
										map.mapData[units[i].x][units[i].y].on = i;
										aEffect.subX = aAtack.x;
										aEffect.subY = aAtack.y;
										aEffect.subNum = (abs(aEffect.x - aEffect.subX) + abs(aEffect.x - aEffect.subY)) * aAtack.moveLag / units[i].spd;
									}
									if (aAtack.x < 0 || 20 <= aAtack.x || aAtack.y < 0 || 13 <= aAtack.y) { break; }
									attackTask.push_back(aAtack);
								}


							}
							units[i].kouchoku = 90 / units[i].spd + 10;
						}
						//通常攻撃の処理
						else {
							int num = units[i].attackList.size();
							//突進以外の攻撃処理。　アタックリストの基礎データに追加情報を与えて処理配列に追加
							if (units[i].attackList[0].tosshin == 0) {
								for (int j = 0; j < num; j++) {
									attack aAtack = units[i].attackList[j];
									aAtack = SettingAttack(aAtack, i);
									if (aAtack.x < 0 || 20 <= aAtack.x || aAtack.y < 0 || 13 <= aAtack.y) { break; }
									attackTask.push_back(aAtack);
								}
							}
							//突進攻撃の処理。　ユニットを突進モードに
							else {
								//units[i].tossin = units[i].attackList[0].tosshin;

								std::vector<attack> tmpList(TosshinCur(units[i].chargeList, i));
								for (int j = 0; j < num; j++) {
									attack aAtack = tmpList[j];
									if (aAtack.aType == T_MAIN) {
										map.mapData[units[i].x][units[i].y].on = -1;
										units[i].x = aAtack.x - urdlX[units[i].bectle];
										units[i].y = aAtack.y - urdlY[units[i].bectle];
										map.mapData[units[i].x][units[i].y].on = i;
										aEffect.subX = aAtack.x;
										aEffect.subY = aAtack.y;
										aEffect.subNum = (abs(aEffect.x - aEffect.subX) + abs(aEffect.x - aEffect.subY)) * aAtack.moveLag / units[i].spd;

									}
									if (aAtack.x < 0 || 20 <= aAtack.x || aAtack.y < 0 || 13 <= aAtack.y) { break; }
									attackTask.push_back(aAtack);
								}


							}


							units[i].kouchoku = 30 / units[i].spd + 2;
						}
						units[i].charge = 0;
						effectList.push_back(aEffect);
					}
					//チャージ
					else if (controls[i].Hold(K_A)) {
						if (units[i].charge < 10000)units[i].charge++;
					}


				}
				units[i].FlameTask();
			}

			//勝利条件
			if (mode <= 1) {
				int liveCount = 0;
				int liver;
				for (int i = 0; i < playerNum; i++) {
					if (units[i].live) {
						liveCount++;
						liver = i;
					}
				}
				if (liveCount == 1) {
					sokomade = 180;
					runk[liver] = 1;
				}

			}
			if (mode == 2) {
				bool rLive = false, bLive = false;
				for (int i = 0; i < playerNum; i++) {
					if (units[i].aSide == S_RED && units[i].live == true) rLive = true;
					if (units[i].aSide == S_BLUE && units[i].live == true)  bLive = true;

				}
				if (rLive == false) map.mapData[map.redPoint.x][map.redPoint.y].Hp = 0;
				if (bLive == false) map.mapData[map.bluePoint.x][map.bluePoint.y].Hp = 0;

				int on = map.mapData[map.redPoint.x][map.redPoint.y].on;
				if (on != -1) {
					if (units[on].aSide != S_RED) {
						sokomade = 180;
						runk[0] = runk[1] = 4;
						runk[2] = runk[3] = 1;
					}
				}
				on = map.mapData[map.bluePoint.x][map.bluePoint.y].on;
				if (on != -1) {
					if (units[on].aSide != S_BLUE) {
						sokomade = 180;
						runk[0] = runk[1] = 1;
						runk[2] = runk[3] = 4;
					}
				}
			}
		}

		//終了アニメーション
		else {
			sokomade--;
			if (sokomade == 0) {
				nextKey[0] = 3;
				nextKey[1] = mode;
				for (int i = 0; i < 4; i++) {
					nextKey[2 + i] = units[i].myNumber;
					nextKey[6 + i] = runk[i];
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
ゲーム画面でのもろもろの描画
*/
void GameModule::Draw() {
	//マップの描画
	map.DrawMap(40, 0);	

	//攻撃予測の描画
	int num = attackTask.size();
	for (int i = 0; i < num; i++) {
		if (attackTask[i].lagFlame < 30) {
			int x = attackTask[i].x * 40;
			int y = attackTask[i].y * 40 - 40;
			int aColor;
			if (attackTask[i].aSide == S_RED) aColor = RED;
			else if (attackTask[i].aSide == S_BLUE) aColor = BLUE;
			else if (attackTask[i].aSide == S_GREEN) aColor = GREEN;
			else aColor = YELLOW;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, int(attackTask[i].power * 2) + 5);
			DrawBox(x - 1, y - 1, x + 41, y + 41, aColor, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			DrawBox(x - 1, y - 1, x + 41, y + 41, aColor, false);
		}
	}

	//ユニットの描画
	for (int i = 0; i < playerNum; i++) {
		units[i].DrawMapUnit(40, 0);
	}
	//各エフェクト類
	PlayEffect();
	DrawDmeffect();

	//アイテム
	drawItems();

	//ステータスUI
	for (int i = 0; i < playerNum; i++) {
		int x = i * 200;
		int y = 450;
		DrawStates(i, x, y);
	}

	//終了アニメーション
	int x;
	if (sokomade > 0) {
		if (sokomade > 160) x = -400 + (180 - sokomade) * 30;
		else x = 200;
		DrawStringToHandle(x, 200, "そこまで！",WHITE,animeFont);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50 + (60 - sokomade) * 4);
		DrawBox(0, 0, 800, 600, BLACK, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

/*
モードとユニットの順番から何陣営か回答する
*/
side GameModule::WariateSide(int num) {
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

//座標をベクトル方向に回転させる
point GameModule::Around(point aPoint, int aBectle) {
	point ans;
	if (aBectle == 1) {
		ans.x = aPoint.y;
		ans.y = -aPoint.x;
	}
	else if (aBectle == 0) {
		ans.x = -aPoint.x;
		ans.y = -aPoint.y;
	}
	else if (aBectle == 3) {
		ans.x = -aPoint.y;
		ans.y = aPoint.x;
	}
	else {
		ans = aPoint;
	}
	return ans;
}

/*
攻撃の処理
*/
void GameModule::AtackCur() {
	int num = attackTask.size();
	for (int i = 0; i < num; i++) {
		attack aAtack = attackTask[i];
		int x = aAtack.x;
		int y = aAtack.y;
		if (aAtack.lagFlame == 0) {
			int on = map.mapData[x][y].on;
			//敵ユニットへの攻撃HIT
			if (on >= 0 && units[on].aSide != aAtack.aSide) {
				int damage = int(aAtack.power / pow(1.4142, units[on].def));
				if (units[on].piyopiyo > 0) damage /= 4;
				else if (units[on].muteki > 0) damage *= 0;
				else 	units[on].hirumi += damage;
				units[on].hitPoint -= damage;
				addDmeffect(damage, x, y);
				if (units[on].hitPoint <= 0) {
					units[on].live = false;
					map.mapData[x][y].on = -1;
					runk[on] = 4 - deadNum;
					deadNum++;
					item aItem = { GetRand(3),x,y };
					items.push_back(aItem);
				}
				aAtack.range = 1;
			}
			//拠点への攻撃HIT
			if (((map.mapData[x][y].aArea == A_RED && aAtack.aSide == S_BLUE) ||
				(map.mapData[x][y].aArea == A_BLUE && aAtack.aSide == S_RED)) && map.mapData[x][y].Hp > 0) {
				int damage = int(aAtack.power / pow(1.4142, 3));
				map.mapData[x][y].Hp -= damage;
				addDmeffect(damage, x, y);
			}

			if (map.mapData[x][y].aArea == A_WALL) {
				aAtack.range = 1;
			}
			aAtack.range--;
			if (aAtack.range > 0) {
				attack addAtack = aAtack;
				addAtack.x += addAtack.nextX;
				addAtack.y += addAtack.nextY;
				addAtack.lagFlame = addAtack.moveLag;
				attackTask.push_back(addAtack);
			}
			attackTask.erase(attackTask.begin() + i);
			i--;
			num--;


		}
		else {
			attackTask[i].lagFlame--;
		}
	}
}

/*
攻撃エフェクトの再生
*/
void GameModule::PlayEffect() {
	int num = effectList.size();
	for (int i = 0; i < num; i++) {
		effect aEffect = effectList[i];
		switch (effectList[i].type) {
		case 0: {
			static int graphic = LoadGraph("graph/effect/pilar.png");
			int num = aEffect.flame / 3;
			int tmpGraph = DerivationGraph(num * 240, 0, 240, 360, graphic);
			int x = aEffect.x * 40 + urdlX[aEffect.bectle] * 0 + 20;
			int y = aEffect.y * 40 - urdlY[aEffect.bectle] * 30 - 20;
			DrawRotaGraph2(x, y, 120, 360, 0.4, aEffect.bectle * PI_2, tmpGraph, true, true);
			DeleteGraph(tmpGraph);
		}break;

		case 1: {
			static int graphic = LoadGraph("graph/effect/slash2.png");
			int num = aEffect.flame / 4;
			int tmpGraph = DerivationGraph(num * 240, 0, 240, 240, graphic);
			int x = (aEffect.x + urdlX[aEffect.bectle]) * 40 + 20;
			int y = (aEffect.y + urdlY[aEffect.bectle]) * 40 - 20;
			DrawRotaGraph(x, y, 0.4, 1, tmpGraph, true);
			DeleteGraph(tmpGraph);
		}break;
		case 2: {
			static int graphic = LoadGraph("graph/effect/slain.png");
			int num = aEffect.flame / 2;
			int tmpGraph = DerivationGraph(num * 360, 0, 360, 240, graphic);
			int x = (aEffect.x + urdlX[aEffect.bectle]) * 40 + 20;
			int y = (aEffect.y + urdlY[aEffect.bectle]) * 40 - 20;
			DrawRotaGraph(x, y, 0.5, aEffect.bectle * PI_2, tmpGraph, true);
			DeleteGraph(tmpGraph);
		}break;
		case 3: {
			static int graphic = LoadGraph("graph/effect/pilar.png");
			int num = aEffect.flame / 3;
			int tmpGraph = DerivationGraph(num * 240, 0, 240, 360, graphic);
			int x = aEffect.x * 40 + urdlX[aEffect.bectle] * 0 + 20;
			int y = aEffect.y * 40 - urdlY[aEffect.bectle] * 30 - 20;
			DrawRotaGraph2(x, y, 120, 360, 0.4, aEffect.bectle * PI_2, tmpGraph, true, true);
			DeleteGraph(tmpGraph);
		}break;
		case 4: {
			static int graphic = LoadGraph("graph/effect/allow.png");
			int num = aEffect.flame / 3 - 1;
			int tmpGraph = DerivationGraph(num * 240, 0, 240, 240, graphic);
			for (int i = 0; i < 2; i++) {
				int x = (aEffect.x + urdlX[aEffect.bectle] * (i + 2)) * 40 + 20;
				int y = (aEffect.y + urdlY[aEffect.bectle] * (i + 2)) * 40 - 20;
				DrawRotaGraph(x, y, 0.4, 0, tmpGraph, true);
			}
			DeleteGraph(tmpGraph);
		}break;
		case 5: {
			static int graphic = LoadGraph("graph/effect/slash.png");
			for (int i = 0; i < 2; i++) {
				int num = aEffect.flame / 4 - (i * 3);
				int tmpGraph = DerivationGraph(num * 240, 0, 240, 240, graphic);
				int x = (aEffect.x + urdlX[aEffect.bectle]) * 40 + 20;
				int y = (aEffect.y + urdlY[aEffect.bectle]) * 40 - 20;
				DrawRotaGraph(x, y, 0.3, i * PI_2, tmpGraph, true);
				DeleteGraph(tmpGraph);
			}
		}break;
		case 10: {
			static int graphic1 = LoadGraph("graph/effect/circle.png");
			static int graphic2 = LoadGraph("graph/effect/bom.png");
			int num1 = aEffect.flame / 3;
			int num2 = aEffect.flame / 6 - 4;
			int tmpGraph1 = DerivationGraph(num1 * 240, 0, 240, 240, graphic1);
			int tmpGraph2 = DerivationGraph(num2 * 240, 0, 240, 240, graphic2);
			int x = (aEffect.x + urdlX[aEffect.bectle] * 3) * 40 + 20;
			int y = (aEffect.y + urdlY[aEffect.bectle] * 3) * 40 - 20;
			DrawRotaGraph(x, y, 0.7, 0, tmpGraph1, true);
			DrawRotaGraph(x, y, 0.6, 0, tmpGraph2, true);
			DeleteGraph(tmpGraph1);
			DeleteGraph(tmpGraph2);
		}break;
		case 11: {
			static int graphic1 = LoadGraph("graph/effect/iaimae.png");
			static int graphic2 = LoadGraph("graph/effect/nagiharai.png");
			int num1 = aEffect.flame / 3 + 3;
			int num2 = (aEffect.flame - aEffect.subNum) / 3 + 2;
			int tmpGraph1 = DerivationGraph((num1 % 5) * 240, (num1 / 5) * 240, 240, 240, graphic1);
			int tmpGraph2 = DerivationGraph((num2 % 5) * 320, (num2 / 5) * 240, 320, 240, graphic2);
			int x = aEffect.x * 40 + 20;
			int y = aEffect.y * 40 - 20;
			int x2 = (aEffect.subX) * 40 + 20 - urdlX[aEffect.bectle] * 20;
			int y2 = (aEffect.subY) * 40 - 20 - urdlY[aEffect.bectle] * 20;
			DrawRotaGraph(x, y, 0.3, 0, tmpGraph1, true);
			DrawRotaGraph(x2, y2, 0.4, PI + aEffect.bectle*PI_2, tmpGraph2, true);
			DeleteGraph(tmpGraph1);
			DeleteGraph(tmpGraph2);
		}break;
		case 12: {
			static int graphic1 = LoadGraph("graph/effect/hanmer.png");
			static int graphic2 = LoadGraph("graph/effect/rock.png");
			int num1 = aEffect.flame / 6;
			int num2 = aEffect.flame / 7 - 1;
			int num3 = aEffect.flame / 7 - 4;
			int tmpGraph1 = DerivationGraph(num1 * 240, 0, 240, 240, graphic1);
			int tmpGraph2 = DerivationGraph(num2 * 240, 0, 240, 240, graphic2);
			int tmpGraph3 = DerivationGraph(num3 * 240, 0, 240, 240, graphic2);
			int x = (aEffect.x + urdlX[aEffect.bectle]) * 40 + 20;
			int y = (aEffect.y + urdlY[aEffect.bectle]) * 40 - 50;
			DrawRotaGraph(x, y, 0.5, 0, tmpGraph1, true);
			for (int i = 0; i < 3; i++) {
				int x2 = (aEffect.x + urdlX[aEffect.bectle] * 2) * 40 + (i - 1) * 40 * urdlX[(aEffect.bectle + 1) % 4] + 20;
				int y2 = (aEffect.y + urdlY[aEffect.bectle] * 2) * 40 + (i - 1) * 40 * urdlY[(aEffect.bectle + 1) % 4] - 20;
				int x3 = (aEffect.x + urdlX[aEffect.bectle] * 3) * 40 + (i - 1) * 80 * urdlX[(aEffect.bectle + 1) % 4] + 20;
				int y3 = (aEffect.y + urdlY[aEffect.bectle] * 3) * 40 + (i - 1) * 80 * urdlY[(aEffect.bectle + 1) % 4] - 20;
				DrawRotaGraph(x2, y2, 0.4, 0, tmpGraph2, true);
				DrawRotaGraph(x3, y3, 0.35, 0, tmpGraph3, true);
			}
			DeleteGraph(tmpGraph1);
			DeleteGraph(tmpGraph2);
		}break;
		case 13: {
			static int graphic1 = LoadGraph("graph/effect/iaimae.png");
			static int graphic2 = LoadGraph("graph/effect/totugeki.png");
			int num1 = aEffect.flame / 3 + 3;
			int num2 = (aEffect.flame - aEffect.subNum) / 3 + 2;
			int tmpGraph1 = DerivationGraph((num1 % 5) * 240, (num1 / 5) * 240, 240, 240, graphic1);
			int tmpGraph2 = DerivationGraph((num2 % 3) * 320, (num2 / 3) * 240, 320, 240, graphic2);
			int x = aEffect.x * 40 + 20;
			int y = aEffect.y * 40 - 20;
			int x2 = (aEffect.subX) * 40 + 20 + urdlX[aEffect.bectle] * 25;
			int y2 = (aEffect.subY) * 40 - 20 + urdlY[aEffect.bectle] * 25;
			DrawRotaGraph(x, y, 0.3, 0, tmpGraph1, true);
			DrawRotaGraph(x2, y2, 0.6, aEffect.bectle*PI_2, tmpGraph2, true);
			DeleteGraph(tmpGraph1);
			DeleteGraph(tmpGraph2);
		}break;
		case 14: {
			static int graphic = LoadGraph("graph/effect/shoot.png");
			int num = aEffect.flame / 9;
			int tmpGraph = DerivationGraph(num * 240, 0, 240, 240, graphic);
			int x = (aEffect.x) * 40 + 20 + urdlX[aEffect.bectle] * ((aEffect.flame + 7) * 8 - int(2.44 * aEffect.flame));
			int y = (aEffect.y) * 40 - 20 + urdlY[aEffect.bectle] * ((aEffect.flame + 7) * 8 - int(2.44 * aEffect.flame));
			DrawRotaGraph(x, y, 0.5, aEffect.bectle * PI_2, tmpGraph, true);
			DeleteGraph(tmpGraph);
		}break;
		case 15: {
			static int graphic1 = LoadGraph("graph/effect/wind_edge.png");
			static int graphic2 = LoadGraph("graph/effect/wind.png");
			int num1 = aEffect.flame / 4;
			int num2 = aEffect.flame / 6;
			int tmpGraph1 = DerivationGraph(num1 * 240, 0, 240, 240, graphic1);
			int tmpGraph2 = DerivationGraph(num2 * 240, 0, 240, 240, graphic2);
			int x = (aEffect.x + urdlX[aEffect.bectle]) * 40 + 20;
			int y = (aEffect.y + urdlY[aEffect.bectle]) * 40 - 20;
			int x2 = (aEffect.x) * 40 + 20 + urdlX[aEffect.bectle] * aEffect.flame * 5;
			int y2 = (aEffect.y) * 40 - 25 + urdlY[aEffect.bectle] * aEffect.flame * 5;
			DrawRotaGraph(x, y, 0.5, 0, tmpGraph1, true);
			DrawRotaGraph(x2, y2, 0.4, 0, tmpGraph2, true);
			DeleteGraph(tmpGraph1);
			DeleteGraph(tmpGraph2);
		}break;
		}

		effectList[i].flame++;
		if (effectList[i].flame > 180) {
			effectList.erase(effectList.begin() + i);
			i--;
			num--;
		}

	}
}

/*
ダメージエフェクトの登録
*/
void GameModule::addDmeffect(int value, int x, int y) {
	int num = dmeffectList.size();
	dmeffect aEffect = { value,x,y,0 };
	for (int i = 0; i < num; i++) {
		if (dmeffectList[i].x == x && dmeffectList[i].y == y) {
			int flameDef = dmeffectList[i].flame - aEffect.flame;
			if (flameDef < 10) aEffect.flame -= (10 - flameDef);
		}
	}
	dmeffectList.push_back(aEffect);
}

/*
ダメージエフェクトの描画
*/
void GameModule::DrawDmeffect() {
	int num = dmeffectList.size();
	for (int i = 0; i < num; i++) {
		dmeffect aEffect = dmeffectList[i];
		if (aEffect.flame >= 0) {
			int x = aEffect.x * 40 + 15;
			if (aEffect.value >= 10) x -= 10;
			int y = aEffect.y * 40 - 20 - aEffect.flame * 2;
			DrawFormatStringToHandle(x, y, GetColor(255, 255, 255), damageFont, "%d", aEffect.value);
		}
		dmeffectList[i].flame++;
		if (dmeffectList[i].flame > 30) {
			dmeffectList.erase(dmeffectList.begin() + i);
			i--;
			num--;
		}
	}
}


/*
ステータスの描画
*/
void GameModule::DrawStates(int num, int x, int y) {
	static int hpBar = LoadGraph("graph/other/hpbar.png");
	static int statesUps[3] = {
		LoadGraph("graph/other/atkUp.png"),
		LoadGraph("graph/other/defUp.png"),
		LoadGraph("graph/other/spdUp.png")
	};

	if (!units[num].live)SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	units[num].DrawAsimoto(x + 58, y + 48, count / 5, 3);
	units[num].DrawUnit(x + 58, y + 48, 0, 3);
	if (!units[num].live) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawRectGraph(x + 10, y + 96, 0, 0, int(180 * units[num].hitPoint / units[num].maxHp), 20, hpBar, true, false);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < units[num].stateUp[i]; j++) {
			int aX = x + 110 + j * 25;
			int aY = y + i * 30;
			DrawGraph(aX, aY, statesUps[i], true);
		}
	}
}

//攻撃リストを突進後のリストに変換する
std::vector<attack> GameModule::TosshinCur(std::vector<attack> baseList, int unitNum) {
	std::vector<attack> list(baseList);
	Unit aUnit = units[unitNum];
	int num = list.size();
	bool stop = false;
	for (int i = 0; i < num; i++) {
		list[i] = SettingAttack(list[i], unitNum);
	}
	for (int i = 0; i < list[0].tosshin; i++) {
		//あたり判定。　敵を見つけると停止。　メイン判定のみ壁や仲間キャラクターにも反応する。
		for (int j = 0; j < num; j++) {
			attack aAtack = list[j];
			if (aAtack.aType == T_SUBSUB) continue;
			int on = map.mapData[aAtack.x][aAtack.y].on;
			mData aData = map.mapData[aAtack.x][aAtack.y];
			if (((on != -1 && units[on].aSide != aAtack.aSide) || (aData.Hp>0 && aData.aSide != aAtack.aSide)) ||
				(aAtack.aType == T_MAIN && (aData.aArea == A_WALL || aData.Hp > 0 || on != -1))) {
				stop = true;
				break;
			}
		}
		if (stop)break;
		//あたり判定に反応がなければすべての攻撃を前進し、ラグを少し増やす。
		for (int j = 0; j < num; j++) {
			list[j].x += urdlX[aUnit.bectle];
			list[j].y += urdlY[aUnit.bectle];
			list[j].lagFlame += list[j].moveLag;
		}
	}
	return list;

}

/*
攻撃の処理を行う前準備
*/
attack GameModule::SettingAttack(attack aAtack, int num) {
	point aPoint = { aAtack.x,aAtack.y };
	point bPoint = { aAtack.nextX,aAtack.nextY };
	aPoint = Around(aPoint, units[num].bectle);
	bPoint = Around(bPoint, units[num].bectle);
	aAtack.x = units[num].x + aPoint.x;
	aAtack.y = units[num].y + aPoint.y;
	aAtack.nextX = bPoint.x;
	aAtack.nextY = bPoint.y;
	aAtack.aSide = units[num].aSide;
	aAtack.lagFlame += 16 / units[num].spd + 5;
	aAtack.power *= pow(1.4142, double(units[num].atk));
	return aAtack;
}


/*
アイテムの処理
*/
void GameModule::itemCur() {
	int num = items.size();
	for (int i = 0; i < num; i++) {
		item aItem = items[i];
		int on = map.mapData[aItem.x][aItem.y].on;
		if (on != -1) {
			switch (aItem.num) {
			case 0:
				units[on].hitPoint += units[on].maxHp * 0.3;
				if (units[on].hitPoint > units[on].maxHp) units[on].hitPoint = units[on].maxHp;
				break;
			case 1: units[on].atk++; units[on].stateUp[0]++; break;
			case 2: units[on].def++; units[on].stateUp[1]++; break;
			case 3: units[on].spd++; units[on].stateUp[2]++; break;
			}
			items.erase(items.begin() + i);
			num--;
			i--;
		}

	}

	if (count % 1000 == 0) {
		for (int i = 0; i < 10; i++) {
			int x = GetRand(17) + 1;
			int y = GetRand(11) + 1;
			if (map.mapData[x][y].aArea == A_FLAT && map.mapData[x][y].on == -1) {
				item aItem = { GetRand(3),x,y };
				items.push_back(aItem);
				break;
			}
		}
	}
}

/*
アイテムの描画
*/
void GameModule::drawItems() {
	static int itemGraphs[4] = {
		LoadGraph("graph/item/apple.png"),
		LoadGraph("graph/item/meat.png"),
		LoadGraph("graph/item/fish.png"),
		LoadGraph("graph/item/charot.png")
	};

	int num = items.size();
	for (int i = 0; i < num; i++) {
		item aItem = items[i];
		DrawRotaGraph(aItem.x * 40 + 20, aItem.y * 40 - 20, 1.5, 0, itemGraphs[aItem.num], true);
	}
}