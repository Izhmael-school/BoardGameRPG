/*
 * @brief プレイヤーデータ
 * @author Sekino
 */

#ifndef _PLAYERDATA_H_
#define _PLAYERDATA_H_
#pragma once

#include <string>
#include <vector>
#include "Vector/Vector3.h"

class PlayerData{
private:
	// 名前
	std::string playerName;
	// レベル
	int level;
	// 経験値
	int currentExp;
	// レベルアップに必要な経験値
	int levelUpNeedExp;
	// 最大HP
	int maxHp;
	// 現在のHP
	int currentHp;
	// 攻撃力
	int atk;
	// 防御力
	int def;
	// 素早さ
	int spd;
	// 魔法攻撃力
	int mag;
	// 運
	int luk;
	// 生命力
	int vit;
	// お金
	int money;
	// 色
	unsigned int color;
	// マップ上の位置
	Vector3 mapPosition;
	// 持てるアイテムの数
	int maxItems;
	// 持てる魔法の数
	int maxMagics;
	// 持っているアイテムのリスト
	std::vector<int> itemList;
	// 持っている魔法のリスト
	std::vector<int> magicList;
	// 装備のリスト
	std::vector<int> equipList;
	// プレイヤーの状態異常のリスト
	std::vector<int> statusList;
public:
	PlayerData();
	~PlayerData();

public:
	// --- getter / setter ---
	const std::string& GetPlayerName() const { return playerName; }
	void SetPlayerName(const std::string& name) { playerName = name; }

	int GetLevel() const { return level; }
	void SetLevel(int v) { level = v; }

	int GetCurrentExp() const { return currentExp; }
	void SetCurrentExp(int v) { currentExp = v; }

	int GetLevelUpNeedExp() const { return levelUpNeedExp; }
	void SetLevelUpNeedExp(int v) { levelUpNeedExp = v; }

	int GetMaxHp() const { return maxHp; }
	void SetMaxHp(int v) { maxHp = v; }

	int GetCurrentHp() const { return currentHp; }
	void SetCurrentHp(int v) { currentHp = v; }

	int GetAtk() const { return atk; }
	void SetAtk(int v) { atk = v; }

	int GetDef() const { return def; }
	void SetDef(int v) { def = v; }

	int GetSpd() const { return spd; }
	void SetSpd(int v) { spd = v; }

	int GetMag() const { return mag; }
	void SetMag(int v) { mag = v; }

	int GetLuk() const { return luk; }
	void SetLuk(int v) { luk = v; }

	int GetVit() const { return vit; }
	void SetVit(int v) { vit = v; }

	int GetMoney() const { return money; }
	void SetMoney(int v) { money = v; }

	unsigned int GetColor() const { return color; }
	void SetColor(unsigned int v) { color = v; }

	const Vector3& GetMapPosition() const { return mapPosition; }
	void SetMapPosition(const Vector3& v) { mapPosition = v; }
	void SetMapPosition(int _x,int _y) { mapPosition.x = _x; mapPosition.y = _y; }
	void AddMapPosition(const Vector3& v) { mapPosition = Vector3::VAdd(mapPosition, v); }
	void AddMapPosition(int _x,int _y) { mapPosition.x += _x; mapPosition.y += _y; }

	int GetMaxItems() const { return maxItems; }
	void SetMaxItems(int v) { maxItems = v; }

	int GetMaxMagics() const { return maxMagics; }
	void SetMaxMagics(int v) { maxMagics = v; }

	const std::vector<int>& GetItemList() const { return itemList; }
	int GetItemCount() const { return itemList.size(); }
	void AddItem(int itemId) { itemList.push_back(itemId); }
	void RemoveItem(int arrayIndex) { itemList.erase(itemList.begin() + arrayIndex); }
	void ClearItemList() { itemList.clear(); }

	const std::vector<int>& GetMagicList() const { return magicList; }
	int GetMagicCount() const { return magicList.size(); }
	void AddMagic(int magicId) { magicList.push_back(magicId); }
	void RemoveMagic(int arrayIndex) { magicList.erase(magicList.begin() + arrayIndex); }
	void ClearMagicList() { magicList.clear(); }

	const std::vector<int>& GetEquipList() const { return equipList; }
	void AddEquip(int equipId) { equipList.push_back(equipId); }
	void RemoveEquip(int arrayIndex) { equipList.erase(equipList.begin() + arrayIndex); }
	void ClearEquipList() { equipList.clear(); }

	const std::vector<int>& GetStatusList() const { return statusList; }

};
#endif // !_PLAYERDATA_H_