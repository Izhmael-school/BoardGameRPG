/*
 * @brief アイテム、魔法、装備の情報を管理する
 */

#pragma once
#ifndef _ELEMENTDATAMANAGER_H_
#define _ELEMENTDATAMANAGER_H_

#include "../ManagerBase.h"
#include "DesignPattern/Singleton/Singleton.h"
#include "Definition/CommonModule/Json/MyJson.h"

struct ItemData {
	int id = -1;
	std::string name = "error";
	std::string explanation = "error";
	int effectValue = -1;
	int effectID = -1;
};

struct MagicData {
	int id = -1;
	std::string name = "error";
	std::string explanation = "error";
	int effectValue = -1;
	int effectID = -1;
};

struct EquipData {
	int id = -1;
	std::string name = "error";
	std::string explanation = "error";
	int equipSection = -1;
	int value = -1;
};

class ElementDataManager : public ManagerBase, public Singleton<ElementDataManager> {
private:
	json itemData;  // アイテムのデータ
	json magicData; // 魔法のデータ
	json equipData; // 装備のデータ

public:
	ElementDataManager();
    ~ElementDataManager();

public:
	/*
	 * @brief アイテムのデータを読み込む
	 */
	void LoadElementData();

	/*
	 * @brief アイテムのデータを取得する
	 */
	ItemData GetItemData(int _id);

	/*
	 * @brief 魔法のデータを取得する
	 */
	MagicData GetMagicData(int _id);

	/*
	 * @brief 装備のデータを取得する
	 */
	EquipData GetEquipData(int _id);
};

#endif // !_ELEMENTDATAMANAGER_H_