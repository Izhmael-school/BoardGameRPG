/*
 * @brief 防御側のコマンド
 */
#pragma once
#ifndef _DEFENSECOMMAND_H_
#define _DEFENSECOMMAND_H_

class CharacterData;

#include "../../BattleInfo.h"
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

	std::vector<std::string>GetCommandNameArray() const;

	std::vector<bool> GetCanSelectCommand()const { return canSelectCommand; };

	void Reset();
};

#endif