/*
 * @brief プレイヤーデータ
 * @author Sekino
 */

#ifndef _PLAYERDATA_H_ 
#define _PLAYERDATA_H_

#include "../CharacterData.h"


class PlayerData : public CharacterData {
private:
	// レベルアップに必要な経験値
	int levelUpNeedExp = 100;
	// 色
	unsigned int color;
	// 持てるアイテムの数
	int maxItems = 10;
	// 持てる魔法の数
	int maxMagics = 10;
	// 1ターンに使えるアイテムの数
	int oneTurnItemCount = 1;
	// 1ターンに使えるアイテムの数
	int oneTurnMagicCount = 1;
	// ターンに使ったアイテムの数
	int usedItemCount = 0;
	// ターンに使った魔法の数
	int usedMagicCount = 0;
	// 持っているアイテムのリスト
	std::vector<int> itemList;
	// 持っている魔法のリスト
	std::vector<int> magicList;
	// コントローラ番号(-1ならキーボード)
	int controllerNum = -1;
	// ステ振りの回数
	int statusPoint = 0;
	// 動けないターン数
	int dontMoveTurnNum = 0;

public:
	PlayerData();
	~PlayerData() override;

public:
	// --- getter / setter ---
	bool IsLevelUp() const { return currentExp >= levelUpNeedExp; }
	bool LevelUp();
	int GetLevelUpNeedExp() const { return levelUpNeedExp; }
	void SetLevelUpNeedExp(int v) { levelUpNeedExp = v; }
	
	int GetStatusPoint() const const { return statusPoint; }
	void SetStatusPoint(int _v) { statusPoint = _v; }
	void SubStatusPoint(int _v) { statusPoint -= _v; }
	void AddStatusPoint(int _v) { statusPoint += _v; }

	unsigned int GetColor() const { return color; }
	void SetColor(unsigned int v) { color = v; }

	int GetControllerNum() const { return controllerNum; }
	int SetControllerNum(int _controllerNum) { controllerNum = _controllerNum; }

	int GetMaxItems() const { return maxItems; }
	void SetMaxItems(int v) { maxItems = v; }

	int GetMaxMagics() const { return maxMagics; }
	void SetMaxMagics(int v) { maxMagics = v; }

	int GetOneTurnItemCount() const { return oneTurnItemCount; }
	void SetOneTurnItemCount(int _v) { oneTurnItemCount = _v; }
	int GetOneTurnMagicCount() const { return oneTurnMagicCount; }
	void SetOneTurnMagicCount(int _v) { oneTurnMagicCount = _v; }

	int GetUsedItemCount() const { return usedItemCount; }
	void AddUsedItemCount() { usedItemCount++; }
	void ResetUsedItemCount() { usedItemCount = 0; }
	int GetUsedMagicCount() const { return usedMagicCount; }
	void AddUsedMagicCount() { usedMagicCount++; }
	void ResetUsedMagicCount() { usedMagicCount = 0; }

	const std::vector<int>& GetItemList() const { return itemList; }
	int GetItemCount() const { return itemList.size(); }
	void AddItem(int itemId) { itemList.push_back(itemId); }
	void RemoveItem(int arrayIndex) { itemList.erase(itemList.begin() + arrayIndex); }
	void ClearItemList() { itemList.clear(); }
	bool IsUseItem() const { return itemList.size() > 0 && usedItemCount < oneTurnItemCount; }
	bool IsUseMagic() const { return magicList.size() > 0 && usedMagicCount < oneTurnMagicCount; }

	const std::vector<int>& GetMagicList() const { return magicList; }
	int GetMagicCount() const { return magicList.size(); }
	void AddMagic(int magicId) { magicList.push_back(magicId); }
	void RemoveMagic(int arrayIndex) { magicList.erase(magicList.begin() + arrayIndex); }
	void ClearMagicList() { magicList.clear(); }

	int GetDontMoveTurnNum() const { return dontMoveTurnNum; }
	void SetDontMoveTurnNum(int _v) { dontMoveTurnNum = _v; }
	void SubDontMoveTurnNum(int _v = 1) { dontMoveTurnNum -= _v; }
	bool IsMove() const { return dontMoveTurnNum <= 0; }

	bool CanBattling() const { return dontMoveTurnNum <= 0; }
};
#endif // !_PLAYERDATA_H_