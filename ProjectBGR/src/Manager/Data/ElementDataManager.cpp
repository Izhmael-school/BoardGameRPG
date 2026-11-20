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
			id.useExplanation = item["useExplanation"];
			if (item.contains("subEffectValue"))
				id.subEffectValue = item["subEffectValue"];
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
			md.target = magic["target"];
			return md;
		}
	}
	return MagicData();
}

EquipData ElementDataManager::GetEquipData(int _id) {
	if (_id == -1) return EquipData(-1, "-----", "データがない", 0, 0, -1);

	for (const auto& equip : equipData) {
		if (equip["id"] == _id) {
			EquipData ed;

			ed.id = equip["id"];
			ed.name = equip["name"];
			ed.explanation = equip["explanation"];
			ed.value = equip["value"];
			ed.effectValue = equip["effectValue"];
			ed.equipSection = equip["equipSection"];
			return ed;

		}
	}
	return EquipData();
}
