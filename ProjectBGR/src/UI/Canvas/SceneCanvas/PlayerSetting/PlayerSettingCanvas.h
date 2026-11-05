/*
 * @brief プレイヤー設定用のキャンバス
 */
#pragma once
#ifndef _PLAYERSETTINGCANVAS_H_
#define _PLAYERSETTINGCANVAS_H_

#include "../../UICanvasBase.h"
#include "Turn/Part/PlayerSetting/PlayerSettingInfo.h"
#include "ExternalLibrary/Json/Json.hpp"

class UIObject;

class PlayerSettingCanvas : public UICanvasBase {
private:
	PlayerSettingUINeedInfo info;
	UIObject* playerNum = nullptr;
	UIObject* playerName = nullptr;
	UIObject* usePortSet = nullptr;
	UIObject* playerJob = nullptr;
	UIObject* turnOrder = nullptr;

	PlayerSettingPart prevPart = PlayerSettingPart_Invalid;
	
	nlohmann::json jobData;

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

	void OpenUsePortSet();

	void CloseUsePortSet();

	void UpdateUsePortSet();

	void OpenJobSelectSet();

	void CloseJobSelectSet();

	void UpdateJobSelectSet();

	void OpenTurnOrderSet();

	void CloseTurnOrderSet();

	void UpdateTurnOrderSet();
};

#endif