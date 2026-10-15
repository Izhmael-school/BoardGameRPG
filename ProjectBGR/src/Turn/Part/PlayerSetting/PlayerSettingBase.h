/*
 * @brief プレイヤー設定の基底
 */
#pragma once
#ifndef _PLAYERSETTINGBASE_H_
#define _PLAYERSETTINGBASE_H_

#include <functional>
#include "PlayerSettingInfo.h"

class InputManager;

class PlayerSettingBase {
protected:
	// パート変更用イベント
	std::function<void(PlayerSettingPart)> changePartFunc;
	// UIに情報渡す用
	std::function<void(const PlayerSettingUINeedInfo&)> setUIInfoFunc;
	InputManager* input;

public:
	virtual void Init(std::function<void(PlayerSettingPart)> _changePartFunc,std::function<void(const PlayerSettingUINeedInfo&)> _setUIInfoFunc);

	virtual void Setup();

	virtual void Update(float _t);
};

#endif // !_PLAYERSETTINGBASE_H_