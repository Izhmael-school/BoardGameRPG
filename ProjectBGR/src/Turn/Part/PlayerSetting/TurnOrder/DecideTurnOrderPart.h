/*
 * @brief キャラクターの行動順を決める
 * @author Sekino
 */
#pragma once
#ifndef _DECIDETURNORDERPART_H_
#define _DECIDETURNORDERPART_H_

#include "../PlayerSettingBase.h"
#include <vector>
#include <string>

class DecideTurnOrderPart : public PlayerSettingBase {
private:
	std::vector<std::string> characterNameArray;
	int characterNum = 0;
	std::vector<int> orderArray;
	// 0:ランダム 1:自分で決める
	int currentSelect = 0;
	int currentSelectPlayer = 0;
	int confirm = 0;
	TurnOrderSection section;
public:

	void Update(float _t) override;

	void Setup() override;

	void SetCharacterNameArray(const std::vector<std::string>& _characterNameArray) { characterNameArray = _characterNameArray; }

	void SetCharacterNum(int _characterNum) { characterNum = _characterNum; }

	std::vector<int> GetTurnOrder() const { return orderArray; }
private:
	void RandomSelect();

	void SelectMyself();
};

#endif