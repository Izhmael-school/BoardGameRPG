/*
 * @brief 繧｢繧､繝・Β縲・ｭ疲ｳ輔∬｣・ｙ縺ｮ諠・ｱ繧堤ｮ｡逅・☆繧・
 */

#pragma once
#ifndef _ELEMENTDATAMANAGER_H_
#define _ELEMENTDATAMANAGER_H_

#include "../ManagerBase.h"
#include "DesignPattern/Singleton/Singleton.h"
#include "Definition/CommonModule/Json/MyJson.h"

struct ElementDataBase {
	int id = -1;
	std::string name = "error";
	std::string explanation = "error";
	int value = -1;
	int effectValue = -1;
	int effectID = -1;
};

struct ItemData :public ElementDataBase {
	ItemData(int _id, std::string _name, std::string _explanation, int _value, int _effectValue, int _effectID){
		id = _id;
		name = _name;
		explanation = _explanation;
		value = _value;
		effectValue = _effectValue;
		effectID = _effectID;
	}

	ItemData() = default;
};

struct MagicData :public ElementDataBase {
	MagicData(int _id, std::string _name, std::string _explanation, int _value, int _effectValue, int _effectID) {
		id = _id;
		name = _name;
		explanation = _explanation;
		value = _value;
		effectValue = _effectValue;
		effectID = _effectID;
	}

	MagicData() = default;
};

struct EquipData :public ElementDataBase {
	int equipSection = -1;

	EquipData(int _id, std::string _name, std::string _explanation, int _value, int _effectValue, int _equipSection) {
		id = _id;
		name = _name;
		explanation = _explanation;
		value = _value;
		effectValue = _effectValue;
		equipSection = _equipSection;
	}

	EquipData() = default;
};

class ElementDataManager : public ManagerBase, public Singleton<ElementDataManager> {
private:
	json itemData;  // 繧｢繧､繝・Β縺ｮ繝・・繧ｿ
	json magicData; // 鬲疲ｳ輔・繝・・繧ｿ
	json equipData; // 陬・ｙ縺ｮ繝・・繧ｿ

public:
	ElementDataManager();
    ~ElementDataManager();

public:
	/*
	 * @brief 繧｢繧､繝・Β縺ｮ繝・・繧ｿ繧定ｪｭ縺ｿ霎ｼ繧
	 */
	void LoadElementData();

	/*
	 * @brief 繧｢繧､繝・Β縺ｮ繝・・繧ｿ繧貞叙蠕励☆繧・
	 */
	ItemData GetItemData(int _id);

	int GetItemDataNum() const { return itemData.size(); }

	/*
	 * @brief 鬲疲ｳ輔・繝・・繧ｿ繧貞叙蠕励☆繧・
	 */
	MagicData GetMagicData(int _id);

	int GetMagicDataNum() const { return magicData.size(); }

	/*
	 * @brief 陬・ｙ縺ｮ繝・・繧ｿ繧貞叙蠕励☆繧・
	 */
	EquipData GetEquipData(int _id);

	int GetEquipDataNum() const { return equipData.size(); }
};

#endif // !_ELEMENTDATAMANAGER_H_