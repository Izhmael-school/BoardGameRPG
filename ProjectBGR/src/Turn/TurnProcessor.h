/*
 * @brief 繧ｿ繝ｼ繝ｳ縺ｮ螳溯｡悟・逅・
 * @author Sekino
 */

#pragma once
#ifndef _TURNPROCESSOR_H_
#define _TURNPROCESSOR_H_
#include <vector>
#include <memory>
#include "Vector3.h"
#include "Part/BoardGame/BoardGamePart.h"
#include "Part/Battle/BattlePart.h"

class PlayerData;
class CharacterDataManager;
class MapManager;
class BoardGameCanvas;
class UIManager;

enum GamePart {
	GamePart_TurnStart,
	GamePart_Board,
	GamePart_Battle,
	GamePart_TurnEnd,
};

class TurnProcessor {
private:
	// 繧ｿ繝ｼ繝ｳ縺ｮ鬆・分
	int currentTurn;
	// 莉翫・繝励Ξ繧､繝､繝ｼ縺ｮ繝・・繧ｿ
	PlayerData* currentPlayerData;

	// 繧ｿ繝ｼ繝ｳ鬆・ｺ・
	std::vector<int> orderList;
	MapManager* mapManager;
	
	GamePart gamePart;

	// 縺吶＃繧阪￥繝代・繝・
	BoardGamePart boardGamePart;
	// 戦闘パート
	BattlePart battlePart;

	CharacterDataManager* character;

	GameObjectManager* gameObjectManager;

	UIManager* uiManager;

	// キャンバス
	std::unique_ptr<BoardGameCanvas> boardGameCanvas;

public:
	TurnProcessor(MapManager* _mapManager,CharacterDataManager* _characterDataManager,GameObjectManager* _gameObjectManager,UIManager* _uiManager);
	~TurnProcessor();

public:
	/*
	 * @brief 繧ｿ繝ｼ繝ｳ縺ｮ鬆・分繧定ｨｭ螳壹☆繧・
	 */
	void OrderSet(const std::vector<int>& _orderList) { orderList = _orderList; }

	/*
	 * @brief 繧ｿ繝ｼ繝ｳ縺ｮ髢句ｧ句・逅・
	 */
	void TurnStart();

	/*
	 * @brief 譖ｴ譁ｰ
	 */
	void Update(float _t);

	/*
	 * @brief 繧ｿ繝ｼ繝ｳ縺ｮ謠冗判
	 */
	void Render();

	/*
	 * @brief ターン終了処理
	 */
	void TurnEnd();

	/*
	 * @brief 外部からターンを終了させる
	 */
	void TurnEndFunc() { gamePart = GamePart_TurnEnd; }

	/*
	 * @brief 外部からバトルパートに遷移する
	 */
	void StartBattle(CharacterData* _p1, CharacterData* _p2);
};

#endif