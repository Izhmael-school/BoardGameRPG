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
#include "Vector2.h"
#include "CharacterStatus.h"
#include <memory>

class UIContext;

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
	std::string playerName = "NONAME";
	// ステータス
	std::unique_ptr<CharacterStatus> status;
	// お金
	int money = 0;
	// 経験値
	int currentExp;
	// マップ上の位置
	Vector2 mapPosition;
	// 人かCPUか
	bool isCpu = false;
	// 装備のリスト
	std::vector<int> equipList = { -1,-1,-1,-1,-1 };
	// 復活地点
	Vector2 respawnPoint = Vector2(18, 8);
	// UIに情報を渡すコンテキスト
	std::shared_ptr<UIContext> context;
	// 戦闘中
	bool isBattling = false;
public:
	CharacterData();
	virtual ~CharacterData();

public:
	// --- getter / setter ---
	CharacterStatus* GetStatus() const { return status.get(); }

	int GetLevel() const { return status->GetLevel(); }
	void SetLevel(int v) { status->SetLevel(v); }
	void AddLevel(int _v = 1) { status->AddLevel(_v); }

	int GetMaxHp() const { return status->GetMaxHp(); }
	void SetMaxHp() { status->SetMaxHp(); }

	int GetCurrentHp() const { return status->GetCurrentHp(); }
	void SetCurrentHp(int v) { status->SetCurrentHp(v); }
	void Damage(int _v) { status->Damage(_v); }
	void Heal(int _v) { status->Heal(_v); }
	bool IsDead() const { return status->IsDead(); }

	int GetAtk() const { return status->GetAtk(); }
	void SetAtk(int v) { status->SetAtk(v); }
	void AddAtk(int v) { status->AddAtk(v); }

	int GetDef() const { return  status->GetDef(); }
	void SetDef(int v) { status->SetDef(v); }
	void AddDef(int v) { status->AddDef(v); }

	int GetSpd() const { return  status->GetSpd(); }
	void SetSpd(int v) { status->SetSpd(v); }
	void AddSpd(int v) { status->AddSpd(v); }

	int GetMag() const { return  status->GetMag(); }
	void SetMag(int v) { status->SetMag(v); }
	void AddMag(int v) { status->AddMag(v); }

	int GetLuk() const { return  status->GetLuk(); }
	void SetLuk(int v) { status->SetLuk(v); }
	void AddLuk(int v) { status->AddLuk(v); }

	int GetVit() const { return  status->GetVit(); }
	void SetVit(int v) { status->SetVit(v); }
	void AddVit(int v) { status->AddVit(v); }

	std::vector<int> GetAllStatus() const { return status->GetAllStatus(); };

	int GetCurrentExp() const { return currentExp; }
	void SetCurrentExp(int v) { currentExp = v; }
	void AddCurrentExp(int _v) { currentExp += _v; }

	const std::string& GetPlayerName() const { return playerName; }
	void SetPlayerName(const std::string& name) { playerName = name; }

	int GetMoney() const { return money; }
	void SetMoney(int v) { money = v; }
	void AddMoney(int _v) { money += _v; }

	bool IsBattling() const { return isBattling; }
	void SetBattling(bool _isBattling) { isBattling = _isBattling; }

	bool IsCPU() const { return isCpu; }
	void SetCPU(bool _isCpu) { isCpu = _isCpu; }

	const Vector2& GetMapPosition() const { return mapPosition; }
	void SetMapPosition(const Vector2& v) { mapPosition = v; }
	void SetMapPosition(int _x, int _y) { mapPosition.x = _x; mapPosition.y = _y; }
	void AddMapPosition(const Vector2& v) { mapPosition = Vector2::VAdd(mapPosition, v); }
	void AddMapPosition(int _x, int _y) { mapPosition.x += _x; mapPosition.y += _y; }

	const std::vector<std::pair<StatusChange,int>>& GetStatusChangeList() const { return status->GetStatusChangeList(); }

	bool IsUseMagicAttack() { return equipList[EquipPosition_AttackMagic] != -1; }
	bool IsUseMagicDefense() { return equipList[EquipPosition_DefenseMagic] != -1; }
	const std::vector<int>& GetEquipList() const { return equipList; }
	void SetEquip(int _equipID, int _equipSection) { equipList[_equipSection] = _equipID; status->UpdateStatusForEquip(equipList); }
	void AddEquip(int equipId) { equipList.push_back(equipId);status->UpdateStatusForEquip(equipList); }
	void RemoveEquip(int arrayIndex) { equipList.erase(equipList.begin() + arrayIndex);status->UpdateStatusForEquip(equipList); }
	void ClearEquipList() { equipList.clear(); status->UpdateStatusForEquip(equipList);}

	const Vector2& GetRespawnPoint() const { return respawnPoint; }
	void SetRespawnPoint(const Vector2& _point) { respawnPoint = _point; }

	/*
	 * @brief UIに渡す情報を作る
	 */
	virtual void MakeContext();

	std::shared_ptr<UIContext> GetUIContext() const { return context; }
};

#endif // !_CHARACTERDATA_H_