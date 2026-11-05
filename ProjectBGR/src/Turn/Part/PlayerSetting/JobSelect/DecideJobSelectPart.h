/*
 * @brief プレイヤーのジョブの選択を行う
 * @author Sekino
 */
#pragma once
#ifndef _DECIDEJOBSELECTPART_H_
#define _DECIDEJOBSELECTPART_H_

#include "../PlayerSettingBase.h"
#include "Manager/CharacterData/Job.h"

class DecideJobSelectPart : public PlayerSettingBase {
private:
	int characterNum;
	int playerNum;
	int currentCharacter = 0;
	int currentSelectJobIndex = 0;
	std::vector<int> playerUsePortArray;
	std::vector<Job> playerJobArray;
	std::vector < std::string > playerNameArray;

	bool isCPU = false;
	bool isPlayerSelect = false;
	bool isEndCPUSelect = false;
	// 0:ランダム　1:プレイヤーが選ぶ
	int cpuSelect = 0;

public:
	void Update(float _t);

	void Setup();

	void SetCharacterNum(int _characterNum) { characterNum = _characterNum; }

	void SetPlayerNum(int _playerNum) { playerNum = _playerNum; }

	void SetPlayerUsePortArray(const std::vector<int>& _playerUsePortArray) { playerUsePortArray = _playerUsePortArray; }

	std::vector<Job> GetPlayerJobArray() { return playerJobArray; }

	void SetPlayerNameArray(const std::vector<std::string>& _playerNameArray) { playerNameArray = _playerNameArray; }
private:
	bool CPUSelect();

	void Select();

	void Correct();
};

#endif