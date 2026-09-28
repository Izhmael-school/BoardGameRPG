/*
 * @brief 防御側のコマンド
 */
#pragma once
#ifndef _DEFENSECOMMAND_H_
#define _DEFENSECOMMAND_H_

class CharacterData;

enum DefenseSelectCommand {
	DefenseCommand_Invaild = -1,
	DefenseCommand_Guard,
	DefenseCommand_MagicGuard,
	DefenseCommand_Counter,
	DefenseCommand_Surrender,
	DefenseCommand_Max
};

#include <vector>
#include <string>

class DefenseCommand {
private:
	CharacterData* currentDefenseCharacter;
	std::vector<bool> canSelectCommand;
	DefenseSelectCommand selectCommand;
public:
	/*
	 * @brief ターンの開始時初期化
	 */
	void StartTurn(CharacterData* _currentDefenseCharacter);

	/*
	 * @brief コマンドの選択
	 */
	void SelectCommand();

	/*
	 * @brief 選んだコマンドの取得
	 */
	DefenseSelectCommand GetSelectCommand() const { return selectCommand; }

	/*
	 * @brief コマンドを選んだか
	 */
	bool IsSelect() const { return selectCommand != DefenseCommand_Invaild; }

	/*
	 * @brief コマンドの描画
	 */
	void Render(int _defenseSide);

	/*
 * @brief コマンドの名前を取得
 */
	std::string GetSelectCommandName();

	void Reset();
};

#endif