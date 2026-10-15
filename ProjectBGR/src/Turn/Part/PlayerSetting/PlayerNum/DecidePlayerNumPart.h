/*
 * @brief ゲームに参加するプレイヤーの人数決め
 */
#pragma once
#ifndef _DECIDEPLAYERNUMPART_H_
#define _DECIDEPLAYERNUMPART_H_

#include "../PlayerSettingBase.h"

enum CanSelect_DecidePlayerNumPart {
	CanSelect_Player,	// プレイヤーの人数
	CanSelect_NPC,		// CPUの人数
	CanSelect_Next,		// 次の設定に行く
	CanSelect_Max
};

constexpr int PLAYER_MAX = 4;
class DecidePlayerNumPart : public PlayerSettingBase{
private:
	int playerNum = 1;
	int cpuNum = 0;
	int currentSelect = 0;

public:

	void Update(float _t) override;

	void Setup() override;

};

#endif // !_DECIDEPLAYER_NUMPART_H_