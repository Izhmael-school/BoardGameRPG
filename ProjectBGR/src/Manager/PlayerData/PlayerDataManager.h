/*
 * @brief ゲーム中のプレイヤーのデータを管理する
 * @author Sekino
 */
#pragma once
#include "../ManagerBase.h"
#include "DesignPattern/Singleton/Singleton.h"
#include <vector>
#include <memory>

constexpr int MAX_PLAYER = 4;

class PlayerData;

class PlayerDataManager : public ManagerBase,public Singleton<PlayerDataManager> {
private:
	std::vector<std::unique_ptr<PlayerData>> playerDataArray;

public:
	PlayerDataManager();
	~PlayerDataManager();

public:
	/*
	 * @brief プレイヤーデータの作製
	 */
	void CreatePlayer();

	PlayerData* GetPlayerData(int _index);

	int GetPlayerNum();

	/*
	 * @brief 座標指定でプレイヤーを取得
	 */
	std::vector<PlayerData*> GetPlayerDataToMapPos(int _x, int _y);
};

