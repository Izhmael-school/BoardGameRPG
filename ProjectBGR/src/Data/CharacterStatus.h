/*
 * @breif キャラクターのステータス
 * @author Sekino
 */
#pragma once
#ifndef _CHARACTERSTATUS_H_
#define _CHARACTERSTATUS_H_

#include "Definition/CommonModule/Math/MyMath.h"
#include "Definition/Enum/StatusChange.h"
#include <vector>

enum Status {
	Status_Atk,
	Status_Def,
	Status_Vit,
	Status_Mag,
	Status_Spd,
	Status_Luk,
	Status_Max
};

class CharacterStatus {
public:
	// レベル
	int level = 1;
	// 基礎攻撃力
	int baseAtk = 5;
	// 基礎防御力
	int baseDef = 5;
	// 基礎素早さ
	int baseSpd = 5;
	// 基礎魔法攻撃力
	int baseMag = 5;
	// 基礎運
	int baseLuk = 5;
	// 基礎生命力
	int baseVit = 5;
	// 装備攻撃力
	int equipAtk = 0;
	// 装備防御力
	int equipDef = 0;
	// 装備素早さ
	int equipSpd = 0;
	// 装備魔法攻撃力
	int equipMag = 0;
	// 装備運
	int equipLuk = 0;
	// 装備生命力
	int equipVit = 0;
	// 最大HP
	int maxHp = 10;
	// 現在のHP
	int currentHp = maxHp;
	// 最終攻撃力
	int atk;
	// 最終防御力
	int def;
	// 最終生命力
	int vit;
	// 最終魔法攻撃力
	int mag;
	// 最終素早さ
	int spd;
	// 最終運
	int luk;
	// 状態変化のリスト
	std::vector<std::pair<StatusChange, int>> statusChangeList;
public:
	int GetLevel() const { return level; }
	void SetLevel(int v) { level = v; }
	void AddLevel(int _v = 1) { level += _v; }

	int GetMaxHp() const { return GetVit() * 10; }
	void SetMaxHp() { maxHp = GetMaxHp(); }

	int GetCurrentHp() const { return currentHp; }
	void SetCurrentHp(int v) { currentHp = v; }
	void Damage(int _v) { currentHp = MyMath::Max(0, currentHp - _v); }
	void Heal(int _v) { currentHp = MyMath::Min(currentHp + _v, GetMaxHp()); }
	bool IsDead() const { return currentHp <= 0; }

	int GetAtk() const { return (baseAtk * ChangedStatus(AtkDoubling,AtkHalving)) + equipAtk; }
	void SetAtk(int v) { baseAtk = v; }
	void AddAtk(int v) { baseAtk += v; }

	int GetDef() const { return (baseDef * ChangedStatus(DefDoubling,DefHalving)) + equipDef; }
	void SetDef(int v) { baseDef = v; }
	void AddDef(int v) { baseDef += v; }

	int GetSpd() const { return (baseSpd * ChangedStatus(SpdDoubling,SpdHalving)) + equipSpd; }
	void SetSpd(int v) { baseSpd = v; }
	void AddSpd(int v) { baseSpd += v; }

	int GetMag() const { return  (baseMag * ChangedStatus(MagDoubling,MagHalving)) + equipMag; }
	void SetMag(int v) { baseMag = v; }
	void AddMag(int v) { baseMag += v; }

	int GetLuk() const { return  (baseLuk * ChangedStatus(LukDoubling,LukHalving)) + equipLuk; }
	void SetLuk(int v) { baseLuk = v; }
	void AddLuk(int v) { baseLuk += v; }

	int GetVit() const { return  (baseVit * ChangedStatus(AllDoubling,AllHalving)) + equipVit; }
	void SetVit(int v) { baseVit = v; }
	void AddVit(int v) { baseVit += v; }

	const std::vector<std::pair<StatusChange,int>>& GetStatusChangeList() const { return statusChangeList; }

	/*
	 * @brief ステータス変化の追加
	 */
	void AddStatusChange(StatusChange _statusChange, int _turnNum);

	/*
	 * @brief 指定のステータス変化を持っているか
	 */
	bool SearchStatusChange(StatusChange _statusChange) const;

	/*
	 * @brief ステータス変化の更新
	 */
	void UpdateStatusChange();

	/*
	 * @brief バフによるステータス増加を返す
	 */
	float ChangedStatus(StatusChange _increaseChange, StatusChange _halvingChange) const;

	std::vector<int> GetAllStatus() const;

	/*
	 * @brief 装備によるステータス増加の更新
	 */
	void UpdateStatusForEquip(std::vector<int> _equipList);

	// ---UIに渡す用---
	int& GetLevel() { return level; }
	int& GetCurrentHP() { return currentHp; }
	int& GetMaxHP() { return maxHp; }
	int& GetAtkHP() { return atk = GetAtk(); }
	int& GetDefHP() { return def = GetDef(); }
	int& GetVitHP() { return vit = GetVit(); }
	int& GetMagHP() { return mag = GetMag(); }
	int& GetSpdHP() { return spd = GetSpd(); }
	int& GetLukHP() { return luk = GetLuk(); }
};

#endif