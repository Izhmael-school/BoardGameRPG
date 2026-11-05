/*
 * @brief プレイヤーの名前の設定
 */
#pragma once
#include "../PlayerSettingBase.h"
#include "../PlayerSettingInfo.h"
#include <string>
#include <vector>

class DecidePlayerNamePart :public PlayerSettingBase {
private:
	
	int currentX = 0;
	int currentY = 0;
	InputString currentInputString = InputString_Hiragana;

	std::string name;

	int nameMax = 8;
public:
	void Update(float _t) override;

	void Setup() override;

	inline std::string GetName() const { return name; }

	inline std::string GetCPUName(int _i) const { return "CPU" + std::to_string(_i); }
};

