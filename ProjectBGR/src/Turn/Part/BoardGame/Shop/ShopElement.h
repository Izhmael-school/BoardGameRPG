/*
 * @brief ショップ用の構造体等を分離したファイル
 */
#pragma once
#ifndef _SHOPELEMENT_H_
#define _SHOPELEMENT_H_

#include <string>

enum ElementID {
	ElementID_Invalid = -1,
	ElementID_Item,
	ElementID_Magic,
	ElementID_Equip
};

struct ShopElement {
	int id = -1;
	int value = -1;
	ElementID  elementID = ElementID_Invalid;
	std::string elementName = "";
	std::string elementExplanation = "";
};

struct BuyElement : ShopElement {
	int equipSec = -1;
	int upStateValue = -1;
};

struct SellElement : ShopElement {
	int baseArrayIndex = -1;	// 元の配列で何番目か
};

enum ShopState {
	ShopState_Selection = -1,// 何を選ぶか
	ShopState_Buy,// 購入
	ShopState_Sell,// 売却
	ShopState_Steal,// 盗み
	ShopState_Back,// 帰る
	ShopState_Max
};

enum DontBuyReason {
	DontBuyReason_Invalid,
	DontBuyReason_CanBuy,
	DontBuyReason_NoMoney,
	DontBuyReason_NoSpace,
};

/*
 * @brief UIの更新をするのにShopPartから持ってくる必要があるもの
 */
struct ShopUINeedInfo {
	ShopState selection = ShopState_Selection;
	int selectSectionIndex = ShopState_Selection;
	int elementIndex = 0;
	ShopElement selectElement;
	DontBuyReason dontBuyReason = DontBuyReason_Invalid;
};

#endif