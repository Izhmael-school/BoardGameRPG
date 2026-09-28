#include "OffenseCommand.h"
#include "Data/CharacterData.h"
#include "Manager/Input/InputManager.h"
#include "Manager/Data/ElementDataManager.h"
#include "DxLib.h"


void OffenseCommand::StartTurn(CharacterData* _currentOffenseCharacter) {
	// 現在攻撃側のプレイヤーを取得
	currentOffenseCharacter = _currentOffenseCharacter;
	// できうるコマンドを初期化
	canSelectCommand.resize(OffenseCommand_Max);
	for (int i = 0; i < OffenseCommand_Max; i++) {
		canSelectCommand[i] = false;
	}
	selectCommand = OffenseCommand_Invaild;
	// できうるコマンドを登録
	// 通常攻撃
	canSelectCommand[OffenseCommand_Attack] = true;
	// 必殺技
	canSelectCommand[OffenseCommand_FatalAttack] = true;
	// 攻撃魔法を装備していれば攻撃魔法を使えるようにする
	if (_currentOffenseCharacter->IsUseMagicAttack())
		canSelectCommand[OffenseCommand_Magic] = true;
}

void OffenseCommand::SelectCommand() {
	// データがないか選択済みなら帰る
	if (!currentOffenseCharacter || selectCommand != OffenseCommand_Invaild) return;
	// 通常攻撃
	if (canSelectCommand[OffenseCommand_Attack] && InputManager::GetInstance().IsKeyDown(KEY_INPUT_RIGHT))
		selectCommand = OffenseCommand_Attack;
	// 必殺技
	if (canSelectCommand[OffenseCommand_FatalAttack] && InputManager::GetInstance().IsKeyDown(KEY_INPUT_LEFT))
		selectCommand = OffenseCommand_FatalAttack;
	// 魔法攻撃
	if (canSelectCommand[OffenseCommand_Magic] && InputManager::GetInstance().IsKeyDown(KEY_INPUT_UP))
		selectCommand = OffenseCommand_Magic;
}

void OffenseCommand::Render(int _attackSide) {
	// バトルウィンドウの下部にデバッグ用コマンド一覧を描画
	const int winLeft = 192;
	const int winTop = 108;
	const int winRight = 1728;
	const int winBottom = 972;
	const int marginY = 10;
	const int lineHeight = 24;

	// 表示するコマンドの順序とラベル
	const std::vector<std::pair<std::string, OffenseSelectCommand>> cmds = {
		{"→Attack", OffenseCommand_Attack},
		{"↑Magic", OffenseCommand_Magic},
		{"←Fatal", OffenseCommand_FatalAttack}
	};

	// 四等分したときの下側中央（各下側クアドラントの中央）に配置
	int width = winRight - winLeft;
	int height = winBottom - winTop;
	int quadCenterY = winTop + (height * 3) / 4; // 下側中央のY
	int quadCenterX = (_attackSide == 0) ? (winLeft + width / 4) : (winLeft + (width * 3) / 4);

	int totalHeight = (int)cmds.size() * lineHeight;
	int startY = quadCenterY - totalHeight / 2;
	// ウィンドウ内に収める
	if (startY < winTop + marginY) startY = winTop + marginY;
	if (startY + totalHeight > winBottom - marginY) startY = winBottom - marginY - totalHeight;

	for (size_t i = 0; i < cmds.size(); ++i) {
		const auto &label = cmds[i].first;
		OffenseSelectCommand id = cmds[i].second;

		// 選択可能か・選択済みかで色を変える
		int color = 0x808080; // 非選択: グレー
		if (id >= 0 && (size_t)id < canSelectCommand.size() && canSelectCommand[(size_t)id]) color = 0xffffff; // 選択可能: 白
		if (selectCommand == id) color = 0xffcc00; // 選択中: 黄色

		int w = GetDrawStringWidth(label.c_str(), (int)label.size());
		int x = quadCenterX - w / 2; // 中央揃え

		DrawString(x, startY + (int)i * lineHeight, label.c_str(), color);
	}
}

std::string OffenseCommand::GetSelectCommandName() {
	switch (selectCommand) {
	case OffenseCommand_Attack:
		return "通常攻撃";
	case OffenseCommand_Magic:
		return ElementDataManager::GetInstance().GetMagicData(currentOffenseCharacter->GetEquipList()[EquipPosition_AttackMagic]).name;
	case OffenseCommand_FatalAttack:
		return "必殺技";
	}
	return "";
}

void OffenseCommand::Reset() {
	selectCommand = OffenseCommand_Invaild;
}
