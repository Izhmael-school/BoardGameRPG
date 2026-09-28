/*
 * @brief キャラクター全体で共通のデータ
 * @author Sekino
 */
#pragma once
#ifndef _CHARACTERDATA_H_
#define _CHARACTERDATA_H_

#include <string>
#include <vector>
#include "Vector3.h"
#include "Definition/CommonModule/Math/MyMath.h"

enum EquipPosition {
	EquipPosition_Weapon,
	EquipPosition_Armor,
	EquipPosition_Accessory,
	EquipPosition_AttackMagic,
	EquipPosition_DefenseMagic,
	EquipPosition_Max
};

class CharacterData {
protected:
	// 名前
	std::string playerName;
	// レベル
	int level = 1;
	// 攻撃力
	int atk = 5;
	// 防御力
	int def = 5;
	// 素早さ
	int spd = 5;
	// 魔法攻撃力
	int mag = 5;
	// 運
	int luk = 5;
	// 生命力
	int vit = 5;
	// お金
	int money = 0;
	// 最大HP
	int maxHp = 10 * vit;
	// 現在のHP
	int currentHp = maxHp;
	// 経験値
	int currentExp;
	// マップ上の位置
	Vector3 mapPosition;
	// 状態異常のリスト
	std::vector<int> statusList;
	// 戦い途中の相手
	CharacterData* battlePlayer = nullptr;
	// 戦っているか
	bool isBattling = false;
	// 人かCPUか
	bool isCpu = false;
	// 装備のリスト
	std::vector<int> equipList;
public:
	CharacterData();
	virtual ~CharacterData();

public:
	// --- getter / setter ---
	int GetCurrentExp() const { return currentExp; }
	void SetCurrentExp(int v) { currentExp = v; }
	void AddCurrentExp(int _v) { currentExp += _v; }

	const std::string& GetPlayerName() const { return playerName; }
	void SetPlayerName(const std::string& name) { playerName = name; }

	int GetLevel() const { return level; }
	void SetLevel(int v) { level = v; }

	int GetMaxHp() const { return vit * 10; }
	void SetMaxHp() { maxHp = vit * 10; }

	int GetCurrentHp() const { return currentHp; }
	void SetCurrentHp(int v) { currentHp = v; }
	void Damage(int _v) { currentHp = MyMath::Max(0, currentHp - _v); }
	void Heal(int _v) { currentHp = MyMath::Min(currentHp + _v, GetMaxHp()); }
	bool IsDead() const { return currentHp <= 0; }

	int GetAtk() const { return atk; }
	void SetAtk(int v) { atk = v; }
	void AddAtk(int v) { atk += v; }

	int GetDef() const { return def; }
	void SetDef(int v) { def = v; }
	void AddDef(int v) { def += v; }

	int GetSpd() const { return spd; }
	void SetSpd(int v) { spd = v; }
	void AddSpd(int v) { spd += v; }

	int GetMag() const { return mag; }
	void SetMag(int v) { mag = v; }
	void AddMag(int v) { mag += v; }

	int GetLuk() const { return luk; }
	void SetLuk(int v) { luk = v; }
	void AddLuk(int v) { luk += v; }

	int GetVit() const { return vit; }
	void SetVit(int v) { vit = v; }
	void AddVit(int v) { vit += v; }

	int GetMoney() const { return money; }
	void SetMoney(int v) { money = v; }
	void AddMoney(int _v) { money += _v; }

	bool IsCPU() const { return isCpu; }
	void SetCPU(bool _isCpu) { isCpu = _isCpu; }

	bool IsBattling() const { return isBattling; }
	void SetBattling(bool _isBattling) { isBattling = _isBattling; }

	const Vector3& GetMapPosition() const { return mapPosition; }
	void SetMapPosition(const Vector3& v) { mapPosition = v; }
	void SetMapPosition(int _x, int _y) { mapPosition.x = _x; mapPosition.y = _y; }
	void AddMapPosition(const Vector3& v) { mapPosition = Vector3::VAdd(mapPosition, v); }
	void AddMapPosition(int _x, int _y) { mapPosition.x += _x; mapPosition.y += _y; }

	const std::vector<int>& GetStatusList() const { return statusList; }

	CharacterData* GetBattlePlayer() const { return battlePlayer; }
	void SetBattlePlayer(CharacterData* _battlePlayer) { battlePlayer = _battlePlayer; }
	void ResetBattlePlayer() { battlePlayer = nullptr; }

	bool IsUseMagicAttack() { return equipList[EquipPosition_AttackMagic] != -1; }
	bool IsUseMagicDefense() { return equipList[EquipPosition_DefenseMagic] != -1; }
	const std::vector<int>& GetEquipList() const { return equipList; }
	void AddEquip(int equipId) { equipList.push_back(equipId); }
	void RemoveEquip(int arrayIndex) { equipList.erase(equipList.begin() + arrayIndex); }
	void ClearEquipList() { equipList.clear(); }
};

#endif // !_CHARACTERDATA_H_