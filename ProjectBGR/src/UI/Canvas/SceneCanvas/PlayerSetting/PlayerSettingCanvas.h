/*
 * @brief プレイヤー設定用のキャンバス
 */
#pragma once
#ifndef _PLAYERSETTINGCANVAS_H_
#define _PLAYERSETTINGCANVAS_H_

#include "../../UICanvasBase.h"
#include "Turn/Part/PlayerSetting/PlayerSettingInfo.h"

class UIObject;

class PlayerSettingCanvas : public UICanvasBase {
private:
	PlayerSettingUINeedInfo info;
	UIObject* playerNum = nullptr;
	UIObject* playerName = nullptr;

	PlayerSettingPart prevPart;
	
public:
	PlayerSettingCanvas() = default;
	~PlayerSettingCanvas() = default;

public:
	void Init() override;

	void Update(float _t, UIInput& _input) override;

	inline void SetNeedInfo(const PlayerSettingUINeedInfo& _info) { info = _info; };
private:
	void OpenPlayerNum();

	void ClosePlayerNum();

	void UpdatePlayerNum();

	void OpenPlayerName();

	void ClosePlayerName();

	void UpdatePlayerName();

};

#endif