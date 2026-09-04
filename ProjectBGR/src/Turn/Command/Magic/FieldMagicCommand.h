/*
 * @brief フィールド上での魔法を扱うコマンド
 * @author Sekino
 */

#pragma once
#ifndef _MagicCOMMAND_H_
#define _MagicCOMMAND_H_

class PlayerData;

class FieldMagicCommand {
private:
	int selectMagicIndex;
	int useMagicCount;
	PlayerData* currentSelectPlayer;
public:
	/*
	 * @brief 魔法を選択する
	 * @return 選択した場合 true、それ以外は false
	 */
	bool SelectMagic();

	/*
	 * @brief 魔法を使う
	 */
	void UseMagic(int _effectID);

	/*
	 * @brief 描画
	 */
	void Render();

	/*
	 * @brief ターンの開始時に現在のプレイヤーを渡してもらう
	 */
	void SetCurrentTurnPlayer(PlayerData* _playerData) { currentSelectPlayer = _playerData; };

	/*
	 * @brief アイテム使用フラグの初期化
	 */
	void ResetMagicUsed() { useMagicCount = 0; }

	/*
	 * @brief コマンド選択画面に戻る前に初期化
	 */
	bool Return();
};

#endif