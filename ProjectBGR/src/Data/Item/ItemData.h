/*
 * @brief プレイヤーデータ
 * @author Sekino
 */

#ifndef _ITEMDATA_H_
#define _ITEMDATA_H_
#pragma once

#include <string>
class ItemData{
private:
	// アイテム名
	std::string itemName;
	// アイテムの効果ID
	int effectId;
	// アイテムの効果量
	int effectValue;
};
#endif