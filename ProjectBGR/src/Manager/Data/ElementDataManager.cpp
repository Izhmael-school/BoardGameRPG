#include "ElementDataManager.h"

ElementDataManager::ElementDataManager() {
	LoadElementData();
}

ElementDataManager::~ElementDataManager() {
}

void ElementDataManager::LoadElementData() {
	itemData = MyJson::LoadJsonFile("res/ExternalFile/Data/ItemData.json");
	magicData = MyJson::LoadJsonFile("res/ExternalFile/Data/MagicData.json");
	equipData = MyJson::LoadJsonFile("res/ExternalFile/Data/EquipmentData.json");
}

ItemData ElementDataManager::GetItemData(int _id) {
	for (const auto& item : itemData) {
		if (item["id"] == _id) {
			ItemData id;

			id.id = item["id"];
			id.name = item["name"];
			id.explanation = item["explanation"];
			id.value = item["value"];
			id.effectValue = item["effectValue"];
			id.effectID = item["effectID"];
			return id;
		}
	}
	return ItemData();
}

MagicData ElementDataManager::GetMagicData(int _id) {
	for (const auto& magic : magicData) {
		if (magic["id"] == _id) {
			MagicData md;

			md.id = magic["id"];
			md.name = magic["name"];
			md.explanation = magic["explanation"];
			md.value = magic["value"];
			md.effectValue = magic["effectValue"];
			md.effectID = magic["effectID"];
			return md;
		}
	}
	return MagicData();
}

EquipData ElementDataManager::GetEquipData(int _id) {
	for (const auto& equip : equipData) {
		if (equip["id"] == _id) {
			EquipData ed;

			ed.id = equip["id"];
			ed.name = equip["name"];
			ed.explanation = equip["explanation"];
			ed.value = equip["value"];
			ed.effectValue = equip["effectValue"];
			return ed;

		}
	}
	return EquipData();
}
