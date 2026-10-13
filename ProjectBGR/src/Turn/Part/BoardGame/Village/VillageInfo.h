/*
 * @brief 村用の構造体等を分離したファイル
 * @author Sekino
 */
#pragma once
#ifndef _VILLAGEINFO_H_
#define _VILLAGEINFO_H_

#include <string>
struct VillageData {
	std::string name = "Error:NoName";
	int ownerPlayerNum = -1;
	int taxValue = -1;
	int rewardTax;
	int villageLevel = 1;
};

enum VillageState {
	VillageState_Select = -1,
	VillageState_GetTax,		// 税を受け取る 
	VillageState_Upgrade,		// 村を強化する
	VillageState_Acquisition,	// 他人の村を買収する
	VillageState_Leave,			// 去る
	VillageState_Max
};

enum CantActReason {
	CantActReason_Invalid,
	CantActReason_DontOwner,
	CantActReason_NotEnoughMoney,
	CantActReason_DontNeed,
};

struct VillageNeedUIInfo {
	VillageData villageData;
	VillageState villageState = VillageState_Select;
	CantActReason reason = CantActReason_Invalid;
	int selectionIndex = -1;
	int modeSelectionIndex = -1;
};
#endif // !_VILLAGEINFO_H_