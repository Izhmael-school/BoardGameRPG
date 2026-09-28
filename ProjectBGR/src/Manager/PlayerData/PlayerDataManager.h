/*
 * @brief 繧ｲ繝ｼ繝荳ｭ縺ｮ繝励Ξ繧､繝､繝ｼ縺ｮ繝・・繧ｿ繧堤ｮ｡逅・☆繧・
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
	 * @brief 繝励Ξ繧､繝､繝ｼ繝・・繧ｿ縺ｮ菴懆｣ｽ
	 */
	void CreatePlayer();

	PlayerData* GetPlayerData(int _index);

	int GetPlayerNum();

	/*
	 * @brief 蠎ｧ讓呎欠螳壹〒繝励Ξ繧､繝､繝ｼ繧貞叙蠕・
	 */
	std::vector<PlayerData*> GetPlayerDataToMapPos(int _x, int _y);
};

