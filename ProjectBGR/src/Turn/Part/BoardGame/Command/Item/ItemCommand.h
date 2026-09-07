/*
 * @brief アイテムを扱うコマンド
 * @author Sekino
 */

#pragma once
#ifndef _ITEMCOMMAND_H_
#define _ITEMCOMMAND_H_

class PlayerData;

class ItemCommand {
private:
	int selectItemIndex;
	int useItemCount;
	PlayerData* currentSelectPlayer;
public:
	/*
	 * @brief アイテムを選択する
	 * @return 選択した場合 true、それ以外は false
	 */
	bool SelectItem();

	/*
	 * @brief アイテムを使う
	 */
	void UseItem(int _effectID);

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
	void ResetItemUsed() { useItemCount = 0; }

	/*
	 * @brief コマンド選択画面に戻る前に初期化
	 */
	bool Return();
};

#endif