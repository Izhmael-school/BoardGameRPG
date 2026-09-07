/*
 * @brief ターンの実行処理
 * @author Sekino
 */

#pragma once
#ifndef _TURNPROCESSOR_H_
#define _TURNPROCESSOR_H_
#include <vector>
#include "Vector/Vector3.h"
#include "Part/BoardGame/BoardGamePart.h"

class PlayerData;
class MapManager;

enum GamePart {
	GamePart_Board,
	GamePart_Battle
};

class TurnProcessor {
private:
	// ターンの順番
	int currentTurn;
	// 今のプレイヤーのデータ
	PlayerData* currentPlayerData;

	// ターン順序
	std::vector<int> orderList;
	MapManager* mapManager;
	
	GamePart gamePart;

	// すごろくパート
	BoardGamePart boardGamePart;
public:
	TurnProcessor(MapManager* _mapManager);
	~TurnProcessor();

public:
	/*
	 * @brief ターンの順番を設定する
	 */
	void OrderSet(const std::vector<int>& _orderList) { orderList = _orderList; }

	/*
	 * @brief ターンの開始処理
	 */
	void TurnStart();

	/*
	 * @brief 更新
	 */
	void Update(float _t);

	/*
	 * @brief ターンの描画
	 */
	void Render();

	void TurnEnd();
};

#endif