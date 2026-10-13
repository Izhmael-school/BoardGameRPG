/*
 * @brief 教会の機能
 * @author Sekino
 */
#pragma once
#ifndef _CHURCHPART_H_
#define _CHURCHPART_H_

#include "ChurchInfo.h"
#include "Vector2.h"
#include <functional>
#include <memory>

class CharacterData;
class InputManager;
class UIManager;
class UICanvasBase;

constexpr int WARP_NEED_MONEY = 10000;
constexpr int RECOVERY_NEED_MONEY = 2000;

class ChurchPart {
private:
	CharacterData* currentTurnPlayer = nullptr;

	ChurchState churchState = ChurchState_Selection;

	InputManager* inputManager;

	int currentTurnPlayerPadIndex = -1;

	UIManager* uiManager;

	std::function<void()> leaveFunc;

	std::unique_ptr<UICanvasBase> churchCanvas;

	int selection = 0;

	int commandSelect = 0;

	DontReceiveServiceReason reason;

	std::vector<ChurchWarpNeedInfo> canWarpChurchs;
public:
	/*
	 * @brief 初期化
	 */
	void Init(UIManager* _uiManager,std::function<void()> _leaveFunc);

	/*
	 * @brief 更新
	 */
	void Update(float _t);

	/*
	 * @brief 教会を開く
	 */
	void Open(CharacterData* _currentTurnPlayer);

	/*
	 * @brief 教会を閉じる
	 */
	void Close();

	std::vector<ChurchWarpNeedInfo> GetCanWarpChurchs() const { return canWarpChurchs; }
private:
	/*
	 * @brief 復活場所を設定する
	 */
	void SetRespawnPoint();

	/*
	 * @brief 教会から教会にワープする
	 */
	void ChurchWarp(Vector2 _warpPos);

	/*
	 * @brief 転移できる教会を探す
	 */
	void SearchCanWarpChurch();
	
	/*
	 * @brief HPを回復する
	 */
	void HPRecovery();

	/*
	 * @brief 選択の更新
	 */
	void Update_Selection(float _t);

	/*
	 * @brief 復活場所設定の更新
	 */
	void Update_SetRP(float _t);

	/*
	 * @brief ワープの更新
	 */
	void Update_Warp(float _t);

	/*
	 * @brief 回復の更新
	 */
	void Update_Recovery(float _t);
};

#endif // !_CHURCHPART_H_