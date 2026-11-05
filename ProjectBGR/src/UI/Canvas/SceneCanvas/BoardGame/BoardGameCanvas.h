/*
 * @brief ボードゲームパートのキャンバス
 * @author Sekino
 */
#pragma once
#ifndef _BOARDGAMECANVAS_H_
#define _BOARDGAMECANVAS_H_

#include "../../UICanvasBase.h"
#include "Turn/Part/BoardGame/BoardGameInfo.h"
#include <vector>
#include <string>

class UIText;
class UITextDialog;
class PlayerData;



class BoardGameCanvas  : public UICanvasBase{
private:
	PlayerUIInfo info;
	BoardGameUINeedInfo binfo;
	TurnState prevState;
	int rouletteNum;
	int selectIndex;
	UIText* playerNameUI;
	UIText* moneyUI;
	UITextDialog* explanationUI;
	UIObject* itemListUI;
	UIObject* magicListUI;
	UIObject* moveCommandUI;
	UIObject* commandSelectUI;
	UIObject* searchTileUI;
	UIObject* inBattleUI;
	UIObject* tileEffectUI;
	bool isDialogTextSend;
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

	void OpenMoveCommandUI();

	void UpdateMoveCommandUI();

	void CloseMoveCommandUI();

	void OpenCommandSelectUI();

	void UpdateCommandSelectUI();

	void CloseCommandSelectUI();

	void OpenSearchTileUI();

	void UpdateSearchTileUI();

	void CloseSearchTileUI();

	void OpenInBattleUI();

	void UpdateInBattleUI();

	void CloseInBattleUI();

	void OpenTileEffectUI();

	void UpdateTileEffectUI();

	void CloseTileEffectUI();

	void SetBoardGameUINeedInfo(BoardGameUINeedInfo _info) { binfo = _info; };

	void UIClose();
};

#endif // !_BOARDGAMECANVAS_H_