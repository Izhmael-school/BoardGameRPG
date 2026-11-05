#include "DefenseCommand.h"
#include "Data/CharacterData.h"
#include "Manager/Input/InputManager.h"
#include "DxLib.h"
#include "Manager/Data/ElementDataManager.h"
#include "Data/Player/PlayerData.h"

void DefenseCommand::Render(int _attackSide) {
	// バトルウィンドウの矩形
	const int winLeft = 192;
	const int winTop = 108;
	const int winRight = 1728;
	const int winBottom = 972;
	const int marginY = 10;
	const int lineHeight = 24;

	const std::vector<std::pair<std::string, DefenseSelectCommand>> cmds = {
		{"→ガード", DefenseCommand_Guard},
		{"↑魔法ガード", DefenseCommand_MagicGuard},
		{"←カウンター", DefenseCommand_Counter},
		{"↓降参", DefenseCommand_Surrender}
	};

	int width = winRight - winLeft;
	int height = winBottom - winTop;
	int quadCenterY = winTop + (height * 3) / 4; // 下側中央のY
	int quadCenterX = (_attackSide == 0) ? (winLeft + width / 4) : (winLeft + (width * 3) / 4);

	int totalHeight = (int)cmds.size() * lineHeight;
	int startY = quadCenterY - totalHeight / 2;
	if (startY < winTop + marginY) startY = winTop + marginY;
	if (startY + totalHeight > winBottom - marginY) startY = winBottom - marginY - totalHeight;

	for (size_t i = 0; i < cmds.size(); ++i) {
		const auto &label = cmds[i].first;
		DefenseSelectCommand id = cmds[i].second;

		int color = 0x808080; // 非選択: グレー
		if (id >= 0 && (size_t)id < canSelectCommand.size() && canSelectCommand[(size_t)id]) color = 0xffffff;
		//if (selectCommand == id) color = 0xffcc00;

		int w = GetDrawStringWidth(label.c_str(), (int)label.size());
		int x = quadCenterX - w / 2;

		DrawString(x, startY + (int)i * lineHeight, label.c_str(), color);
	}
}

std::string DefenseCommand::GetSelectCommandName() {
	switch (selectCommand) {
	case DefenseCommand_Guard:
		return "防御";
	case DefenseCommand_MagicGuard:
		return ElementDataManager::GetInstance().GetMagicData(currentDefenseCharacter->GetEquipList()[EquipPosition_DefenseMagic]).name;
	case DefenseCommand_Counter:
		return "カウンター";
	}
	return "";
}

void DefenseCommand::Reset() {
	selectCommand = DefenseCommand_Invaild;
}

void DefenseCommand::StartTurn(CharacterData* _currentDefenseCharacter) {
	// 現在攻撃側のプレイヤーを取得
	currentDefenseCharacter = _currentDefenseCharacter;
	// できうるコマンドを初期化
	canSelectCommand.resize(DefenseCommand_Max);
	for (int i = 0; i < DefenseCommand_Max; i++) {
		canSelectCommand[i] = false;
	}
	selectCommand = DefenseCommand_Invaild;
	// できうるコマンドを登録
	// 防御
	canSelectCommand[DefenseCommand_Guard] = true;
	// カウンター
	canSelectCommand[DefenseCommand_Counter] = true;
	// 防御魔法を装備していれば防御魔法を使えるようにする
	if (_currentDefenseCharacter->IsUseMagicDefense() != -1)
		canSelectCommand[DefenseCommand_MagicGuard] = true;
	// 降参
	canSelectCommand[DefenseCommand_Surrender] = true;
}

void DefenseCommand::SelectCommand() {
	// データがないか選択済みなら帰る
	if (!currentDefenseCharacter || selectCommand != DefenseCommand_Invaild) return;

	InputManager* input = &InputManager::GetInstance();
	int port = -1;

	if (currentDefenseCharacter->IsCPU()) {
		// CPUには仮でガードだけを選択させる
		selectCommand = DefenseCommand_Guard;
		return;
	}
	else {
		
		PlayerData* p = static_cast<PlayerData*>(currentDefenseCharacter);
		// プレイヤーであればコントローラーのポート番号をもらう
		if (p)
			port = p->GetControllerNum();
	}


	// 通常防御
	if (canSelectCommand[DefenseCommand_Guard] && input->IsButtonDown(port,XINPUT_DPAD_RIGHT))
		selectCommand = DefenseCommand_Guard;
	// カウンター
	if (canSelectCommand[DefenseCommand_Counter] && input->IsButtonDown(port, XINPUT_DPAD_LEFT))
		selectCommand = DefenseCommand_Counter;
	// 魔法防御
	if (canSelectCommand[DefenseCommand_MagicGuard] && input->IsButtonDown(port, XINPUT_DPAD_UP))
		selectCommand = DefenseCommand_MagicGuard;
	// 降参
	if (canSelectCommand[DefenseCommand_Surrender] && input->IsButtonDown(port, XINPUT_DPAD_DOWN))
		selectCommand = DefenseCommand_Surrender;
}
