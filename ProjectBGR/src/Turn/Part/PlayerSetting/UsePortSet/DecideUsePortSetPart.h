/*
 * @brief 各プレイヤーが使うコントローラのポートを決める
 * @author Sekino
 */
#pragma once
#ifndef _DECIDEUSEPORTSETPART_H_
#define _DECIDEUSEPORTSETPART_H_

#include "../PlayerSettingBase.h"
#include <vector>
#include <string>

class DecideUsePortSetPart : public PlayerSettingBase {
private:
	std::vector<std::string> playerName;
	int playerNum = 0;
	int characterNum = 0;
	std::vector<int> playerUsePortArray;

public:
	void Update(float _t) override;

	void Setup() override;

	void SetPlayerName(const std::vector<std::string>& _playerName) { playerName = _playerName; }

	void SetPlayerNum(int _playerNum) { playerNum = _playerNum; }

	std::vector<int> GetPlayerUsePortArray() const { return playerUsePortArray; }

	void SetCharacterNum(int _characterNum) { characterNum = _characterNum; }
};

#endif // !_DECIDEUSEPORTSETPART_H_