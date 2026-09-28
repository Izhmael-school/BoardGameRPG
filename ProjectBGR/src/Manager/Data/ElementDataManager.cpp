#include "ElementDataManager.h"

ElementDataManager::ElementDataManager() {
	LoadElementData();
}

ElementDataManager::~ElementDataManager() {
}

void ElementDataManager::LoadElementData() {
	itemData = MyJson::LoadJsonFile("res/ExternalFile/Data/ItemData.json");
	magicData = MyJson::LoadJsonFile("res/ExternalFile/Data/MagicData.json");
	equipData = MyJson::LoadJsonFile("res/ExternalFile/Data/EquipData.json");
}

ItemData ElementDataManager::GetItemData(int _id) {
	for (const auto& item : itemData) {
		if (item["id"] == _id) {
			return ItemData{
				item["id"],
				item["name"],
				item["explanation"],
				item["effectValue"],
				item["effectID"]
			};
		}
	}
	return ItemData();
}

MagicData ElementDataManager::GetMagicData(int _id) {
	for (const auto& magic : magicData) {
		if (magic["id"] == _id) {
			return MagicData{
				magic["id"],
				magic["name"],
				magic["explanation"],
				magic["effectValue"],
				magic["effectID"]
			};
		}
	}
	return MagicData();
}

EquipData ElementDataManager::GetEquipData(int _id) {
	for (const auto& equip : equipData) {
		if (equip["id"] == _id) {
			return EquipData{
				equip["id"],
				equip["name"],
				equip["explanation"],
				equip["equipSection"],
				equip["value"]
			};
		}
	}
	return EquipData();
}
