/*
 * @brief レベルアップ処理
 */
#pragma once
#ifndef _LEVELUPPHASE_H_
#define _LEVELUPPHASE_H_

class PlayerData;

constexpr int canUpStatus = 6;

#include <vector>

class LevelUpPhase {
private:
	static PlayerData* levelUpPlayer;
	static int currentSelectStatus;
	static std::vector<int> upStatus;
	static int remainingStatusPoint;
	static int upLevelIndex;
	static bool start;
	static bool isConfirm;
	static int confirmIndex;
public:
	static void Render();

	/*
	 * @brief レベルアップ処理
	 */
	static bool LevelUp();

	/*
	 * @brief レベルアップするプレイヤーのデータを受け取る
	 */
	static void SetLevelUpPlayer(PlayerData* _levelUpPlayer) { levelUpPlayer = _levelUpPlayer; }

	/*
	 * @brief 上がったレベル数を返す
	 */
	static int GetUpLevelIndex() { return upLevelIndex; }

	/* 
	 * @brief 上昇させるステータスの値を返す
	 */
	static std::vector<int> GetUpStatus() { return upStatus; }

	/*
	 * @brief 残りのポイントを返す
	 */
	static int GetRemainingPoint() { return remainingStatusPoint; }

	/*
	 * @brief ステ振りを開始したか
	 */
	static bool IsStartSortStatus() { return start; }

	/*
	 * @brief 選択中のインデクスを返す
	 */
	static int GetCurrentSelectIndex() { return currentSelectStatus; }

	/*
	 * @brief 確認中か
	 */
	static bool IsConfirm() { return isConfirm; }

	/*
	 * @brief 確認コマンドの番号を返す
	 */
	static int GetConfirmIndex() { return confirmIndex; }
};

#endif // !_LEVELUPPHASE_H_