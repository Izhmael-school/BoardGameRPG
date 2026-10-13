/*
 * @brief ボードゲーム内の村マス
 * @author Sekino
 */
#pragma once
#ifndef _VILLAGEPART_H_
#define _VILLAGEPART_H_

#include "VillageInfo.h"
#include <vector>
#include <utility>

class PlayerData;
class UICanvasBase;
class UIManager;

class VillagePart {
private:
	PlayerData* currentTurnPlayer;
	int tax;
	int reward;
	std::vector<std::pair<std::pair<int,int>,VillageData>> villageDataArray;
	std::pair<int, int> a;

	bool isSaveTax = false;

	VillageData& currentVillage;

	VillageState villageState;

	int modeSelection = 0;

	int elementSelection = 0;

	UIManager* uiManager;

	std::unique_ptr<UICanvasBase> villageCanvas = nullptr;

public:
	void Init(UIManager* _uiManager);

	/*
	 * @brief 更新
	 */
	void Update(float _t);

	/*
	 * @brief 村に入る
	 */
	void Enter(PlayerData* _currentTurnPlayer);

private:
	/*
	 * @brief 村を去る
	 */
	void Leave();

	void Update_GetTax(float _t);
	
	void Update_Upgrade(float _t);

	void Update_Acquisition(float _t);

	void GetTax();

	void Upgrade();

	void Acquisition();

	/*
	 * @brief 週が変わったときに売れとれる税を増やす
	 */
	void AddTax();

	/*
	 * @brief 現在村にいるプレイヤーがオーナーか
	 */
	bool IsOwner() { return currentTurnPlayer->GetPlayerNum() == currentVillage.ownerPlayerNum; }
};

#endif // !_VILLAGEPART_H_