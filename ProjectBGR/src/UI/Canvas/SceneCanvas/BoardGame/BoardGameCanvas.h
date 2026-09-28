/*
 * @brief ボードゲームパートのキャンバス
 * @author Sekino
 */
#pragma once
#ifndef _BOARDGAMECANVAS_H_
#define _BOARDGAMECANVAS_H_

#include "../../UICanvasBase.h"
#include <vector>
#include <string>

class UIText;
class PlayerData;

struct PlayerUIInfo {
	std::string playerName;
	int atk;
	int def;
	int vit;
	int mag;
	int spd;
	int luk;
	int money;
	std::vector<int> itemList;
	std::vector<int> magicList;
	std::vector<int> equipList;
};

class BoardGameCanvas  : public UICanvasBase{
private:
	PlayerUIInfo info;
	int rouletteNum;
	int selectIndex;
	UIText* playerNameUI;
	UIText* moneyUI;
	UIText* explanationUI;
	UIObject* itemListUI;
	UIObject* magicListUI;

public:
	void Init() override;

	void Update(float _t, UIInput& _input) override;

	void SetCurrentTurnPlayerData(PlayerData* _currentTurnPlayer);

	void SetRouletteNum(int _num) { rouletteNum = _num; }

	void SetSelectIndex(int _selectIndex) { selectIndex = _selectIndex; }

	void OpenItemListUI();

	void CloseItemListUI();

	void UpdateItemListUI();

	void OpenMagicListUI();

	void CloseMagicListUI();

	void UpdateMagicListUI();

	void OpenRouletteUI();
};

#endif // !_BOARDGAMECANVAS_H_