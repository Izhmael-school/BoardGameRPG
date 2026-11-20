/*
 * @brief 戦闘パートのキャンバス
 * @author Sekino
 */
#pragma once
#ifndef _BATTLECANVAS_H_
#define _BATTLECANVAS_H_

#include "../../UICanvasBase.h"
#include "Turn/Part/Battle/BattleInfo.h"
#include <functional>
#include <vector>

class CharacterData;
class UIObject;

struct CanvasUIEvent {
	std::function<void()> openUIFunc;
	std::function<void(UIInput&)> updateUIFunc;
	std::function<void()> closeUIFunc;
};

constexpr int BATTLE_CHARACTER_NUM = 2;

class BattleCanvas : public UICanvasBase {
private:
	BattleUINeedInfo info;
	BattleState currentState;
	BattleState prevState;
	std::vector<CanvasUIEvent> stateFunc;
	std::vector<CharacterData*> battleCharacters;
	UIObject* attackOrderUI;
	UIObject* selectCommandUI;
	UIObject* attackConfirmUI;
	UIObject* turnChangeUI;
	UIObject* resultUI;
	UIObject* alwaysUI;
	UIObject* levelUpUI;
	bool isSendTextDialog;
	int attackCharacterIndex;
public:
	void Init() override;

	void Update(float _t, UIInput& _input) override;

	inline void SetBattleCharacterData(std::vector<CharacterData*> _battleCharacters) { battleCharacters = _battleCharacters; };

	inline void SetBattleUINeedInfo(BattleUINeedInfo _info) { info = _info; }

private:
	/*
	 * @brief 状態が変わったらUIを切り替える
	 */
	void UICloseDifferentStateReason();

	void OpenAttackOrderUI();

	void UpdateAttackOrderUI(UIInput& _uiInput);

	void CloseAttackOrderUI();

	void OpenSelectCommandUI();

	void UpdateSelectCommandUI(UIInput& _uiInput);

	void CloseSelectCommandUI();

	void OpenAttackConfirmUI();

	void UpdateAttackConfirmUI(UIInput& _uiInput);

	void CloseAttackConfirmUI();

	void OpenTurnChangeUI();

	void UpdateTurnChangeUI(UIInput& _uiInput);

	void CloseTurnChangeUI();

	void OpenResultUI();

	void UpdateResultUI(UIInput& _uiInput);

	void CloseResultUI();

	void OpenLevelUpUI();

	void UpdateLevelUpUI(UIInput& _uiInput);

	void OpenAlwaysUI();

	void UpdateAlwaysUI();

	void CloseAlwaysUI();

	UIPtr CreatePlayerInfo(Vector2 _pos, int _color, CharacterData* _characterData, std::string _uiName = "");

	UIPtr CreatePlayerStatusInfo(Vector2 _pos, std::vector<CharacterData*> _characterDataArray, std::string _uiName = "");

	void CleanUp();

};

#endif