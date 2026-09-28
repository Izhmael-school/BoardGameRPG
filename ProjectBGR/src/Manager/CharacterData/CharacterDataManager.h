#pragma once
#include "../ManagerBase.h"
#include <vector>
#include <memory>

class PlayerData;
class EnemyData;
class CharacterData;

using PlayerDataPtr = std::unique_ptr<PlayerData>;
using PlayerDataPtrArray = std::vector<PlayerDataPtr>;
using EnemyDataPtr = std::unique_ptr<EnemyData>;
using EnemyDataPtrArray = std::vector<EnemyDataPtr>;

enum Job {
	Job_Fighter,
	Job_Wizard,
	Job_Thief,
	Job_HeavyFighter,
	Job_Monk,
	Job_MagicFencer,
	Job_Assassin,
	Job_OrdinaryPeople,
};

class CharacterDataManager : public ManagerBase {
private:
	// プレイヤーデータ配列
	PlayerDataPtrArray playerDataArray;
	// 敵データ配列
	EnemyDataPtrArray enemyDataArray;

public:
	CharacterDataManager();
	~CharacterDataManager();

public:
	/*
	 * @brief プレイヤーデータの作製
	 */
	PlayerData* CreatePlayerData(Job _job);

	/*
	 * @brief 番号指定でプレイヤーを取得
	 */
	PlayerData* GetPlayerData(int _index) { return playerDataArray[_index].get(); };

	/*
	 * @brief 番号指定でプレイヤーを取得
	 */
	EnemyData* GetEnemyData(int _index) { return enemyDataArray[_index].get(); };

	/*
	 * @brief プレイヤーの数を取得
	 */
	inline int GetPlayerNum() const { return playerDataArray.size(); };

	/*
	 * @brief 敵の数を取得
	 */
	inline int GetEnemyNum() const { return enemyDataArray.size(); };

	/*
	 * @brief 指定したマスにいるすべてのキャラクターの取得
	 */
	std::vector<CharacterData*> GetCharacterDataToMapPos(int _x, int _y);

	/*
	 * @brief ID指定で敵を生成
	 */
	EnemyData* CreateEnemyData(int _enemyID, int _x, int _y, bool _isBoss = false);

	/*
	 * @brief 削除フラグが立っている敵データを消す
	 */
	void DeleteEnemyData();

	/*
	 * @brief 指定座標のエリアでスポーンする敵を探す
	 */


	/*
	 * @brief デバッグ機能：プレイヤーを4体生成する
	 */
	void DebugCreatePlayerData(Job _p1 = Job_Fighter, Job _p2 = Job_Fighter, Job _p3 = Job_Fighter, Job _p4 = Job_Fighter);

	void DebugRender();
};
