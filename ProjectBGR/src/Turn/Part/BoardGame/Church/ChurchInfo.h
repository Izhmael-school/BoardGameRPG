/*
 * @brief 教会に関する列挙体や構造体を宣言するファイル
 * @author Sekino
 */
#pragma once
#ifndef _CHURCHINFO_H_
#define _CHURCHINFO_H_

enum ChurchState {
	ChurchState_Selection = -1,// 何を選ぶか
	ChurchState_SetRP,// 復活場所を設定する
	ChurchState_Warp,// ワープ
	ChurchState_Recovery,// 回復
	ChurchState_Back,// 帰る
	ChurchState_Max
};

enum DontReceiveServiceReason {
	DontReceiveServiceReason_Invalid = -1,
	DontReceiveServiceReason_ReceiveService,
	DontReceiveServiceReason_NoMoney,
	DontReceiveServiceReason_NotNeed,
};

struct ChurchUINeedInfo {
	ChurchState selection = ChurchState_Selection;
	int selectionSectionIndex = 0;
	int commandSelectIndex = 0;
	DontReceiveServiceReason dontReceiveServiceReason = DontReceiveServiceReason_Invalid;
};


#include "Vector2.h"
#include <string>
struct ChurchWarpNeedInfo {
	Vector2 churchPos;
	std::string churchName;
};


const char const CHURCH_NAME[3][100]{ "西の教会","王国","東の教会" };
constexpr int CHURCH_NUM = 3;
constexpr int CAN_WARP_CHURCH_POS[CHURCH_NUM][2] = { {7, 9}, {18, 8}, {27, 15} };
#endif // !_CHURCHINFO_H_