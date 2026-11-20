#include "CharacterStatus.h"
#include "Manager/Data/ElementDataManager.h"

void CharacterStatus::AddStatusChange(StatusChange _statusChange, int _turnNum) {
	if (_statusChange == StatusChange_Invalid) return;

	// 重複チェック
	for (auto s : statusChangeList) {
		if (s.first != _statusChange) continue;

		// あれば残りターン数を上書きする
		s.second = _turnNum;
		return;
	}
	// なければ追加する
	statusChangeList.emplace_back(std::pair<StatusChange, int>(_statusChange, _turnNum));
}

bool CharacterStatus::SearchStatusChange(StatusChange _statusChange) const {
	if (_statusChange == StatusChange_Invalid) return false;

	for (auto s : statusChangeList) {
		if (s.first != _statusChange) continue;

		return true;
	}
	return false;
}

void CharacterStatus::UpdateStatusChange() {
	for (auto s : statusChangeList) {
		// 数値を1減らす
		s.second--;
	}
	// 数値が0なら消す
	std::erase_if(statusChangeList, [this](std::pair<StatusChange, int> _statusChange) {return _statusChange.second <= 0; });
}

float CharacterStatus::ChangedStatus(StatusChange _increaseChange, StatusChange _halvingChange) const {
	bool isIncreaseChange = SearchStatusChange(_increaseChange);
	bool isHalvingChange = SearchStatusChange(_halvingChange);
	// 指定のステータス増加のバフを持っていれば2倍にする
	if (isIncreaseChange && isHalvingChange)
		return 1;
	else if (isIncreaseChange && !isHalvingChange)
		return 2;
	else if (!isIncreaseChange && isHalvingChange)
		return 0.5f;

	isIncreaseChange = SearchStatusChange(AllDoubling);
	isHalvingChange = SearchStatusChange(AllHalving);

	// すべてのステータス増加のバフを持っていれば2倍にする
	if (isIncreaseChange && isHalvingChange)
		return 1;
	else if (isIncreaseChange && !isHalvingChange)
		return 2;
	else if (!isIncreaseChange && isHalvingChange)
		return 0.5f;

	// なければ等倍
	return 1;
}

std::vector<int> CharacterStatus::GetAllStatus() const {
	std::vector<int> status = { GetAtk(), GetDef(), GetVit(), GetMag(), GetSpd(), GetLuk()};
	return status;
}

const int EQUIP_NUM = 3;

void CharacterStatus::UpdateStatusForEquip(std::vector<int> _equipList) {
	// 装備ステータス変数の初期化
	equipAtk = 0;
	equipDef = 0;
	equipVit = 0;
	equipMag = 0;
	equipSpd = 0;
	equipLuk = 0;

	for (int i = 0; i < EQUIP_NUM; i++) {
		auto equip = ElementDataManager::GetInstance().GetEquipData(_equipList[i]);

		int statusNum = equip.increaseStatus;

		if (statusNum == -1) continue;

		Status s = static_cast<Status>(statusNum);

		switch (s) {
		case Status_Atk:
			equipAtk += equip.effectValue;
			break;
		case Status_Def:
			equipDef += equip.effectValue;
			break;
		case Status_Vit:
			equipVit += equip.effectValue;
			break;
		case Status_Mag:
			equipMag += equip.effectValue;
			break;
		case Status_Spd:
			equipSpd += equip.effectValue;
			break;
		case Status_Luk:
			equipLuk += equip.effectValue;
			break;
		}
	}
}

