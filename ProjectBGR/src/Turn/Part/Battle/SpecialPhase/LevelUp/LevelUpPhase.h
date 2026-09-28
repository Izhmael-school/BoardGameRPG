/*
 * @brief レベルアップ処理
 */
#pragma once
#ifndef _LEVELUPPHASE_H_
#define _LEVELUPPHASE_H_

class PlayerData;

constexpr int canUpStatus = 6;

class LevelUpPhase {
private:
	static PlayerData* levelUpPlayer;
	static int currentSelectStatus;
	static int upStatus[canUpStatus];
	static int remainingStatusPoint;

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
};

#endif // !_LEVELUPPHASE_H_