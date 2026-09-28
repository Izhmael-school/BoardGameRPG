/*
 * @brief 攻撃側のコマンド
 */
#pragma once
#ifndef _OFFENSECOMMAND_H_
#define _OFFENSECOMMAND_H_

class CharacterData;

enum OffenseSelectCommand {
	OffenseCommand_Invaild = -1,
	OffenseCommand_Attack,
	OffenseCommand_Magic,
	OffenseCommand_FatalAttack,
	OffenseCommand_Max
};

#include <vector>
#include <string>

class OffenseCommand {
private:
	CharacterData* currentOffenseCharacter;
	std::vector<bool> canSelectCommand;
	OffenseSelectCommand selectCommand;
public:
	/*
	 * @brief ターンの開始時初期化
	 */
	void StartTurn(CharacterData* _currentOffenseCharacter);

	/*
	 * @brief コマンドの選択
	 */
	void SelectCommand();

	/*
	 * @brief 選んだコマンドの取得
	 */
	OffenseSelectCommand GetSelectCommand() const { return selectCommand; }

	/*
	 * @brief コマンドを選んだか
	 */
	bool IsSelect() {return selectCommand != OffenseCommand_Invaild;}

	/*
	 * @brief コマンドの描画
	 */
	void Render(int _attackSide);

	/*
	 * @brief コマンドの名前を取得
	 */
	std::string GetSelectCommandName();

	void Reset();
};

#endif