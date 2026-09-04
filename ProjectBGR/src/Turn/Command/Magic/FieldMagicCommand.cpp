#include "FieldMagicCommand.h"
#include "Manager/Input/InputManager.h"
#include "Data/Player/PlayerData.h"
#include "Manager/Data/ElementDataManager.h"
#include "DxLib.h"

bool FieldMagicCommand::SelectMagic() {
	// プレイヤーが渡されてないかこのターンすでアイテムを使っていたら帰る
	if (!currentSelectPlayer || currentSelectPlayer->GetOneTurnMagicCount() <= useMagicCount) return Return();

	int magicNum = static_cast<int>(currentSelectPlayer->GetMagicList().size()) - 1;

	InputManager& input = InputManager::GetInstance();

	if (input.IsKeyDown(KEY_INPUT_UP))
		selectMagicIndex = max(0, selectMagicIndex - 2);
	if (input.IsKeyDown(KEY_INPUT_DOWN))
		selectMagicIndex = min(magicNum, selectMagicIndex + 2);
	if (input.IsKeyDown(KEY_INPUT_RIGHT))
		selectMagicIndex = min(magicNum, (selectMagicIndex % 2 == 0) ? selectMagicIndex + 1 : selectMagicIndex);
	if (input.IsKeyDown(KEY_INPUT_LEFT))
		selectMagicIndex = (selectMagicIndex % 2 == 1) ? selectMagicIndex - 1 : selectMagicIndex;

	if (input.IsKeyDown(KEY_INPUT_B))
		return Return();

	if (input.IsKeyDown(KEY_INPUT_RETURN)) {
		// アイテムの使用処理を描く
		UseMagic(currentSelectPlayer->GetMagicList()[selectMagicIndex]);
		return Return();
	}
	return false;
}

void FieldMagicCommand::UseMagic(int _effectID) {
	// アイテムを使った回数を増やす
	useMagicCount++;

	// アイテムを消す
	currentSelectPlayer->RemoveMagic(selectMagicIndex);
}

void FieldMagicCommand::Render() {
	VECTOR offset = VGet(400, 200, 0);
	VECTOR offset2 = VGet(700, 400, 0);
	int stringSize = 20;
	// ウィンドウ
	DrawFillBox(offset.x, offset.y, offset2.x, offset2.y, 0x000000);
	DrawLineBox(offset.x, offset.y, offset2.x, offset2.y, 0xffffff);

	// アイテム名
	int i = 0;
	unsigned int color = 0xffffff;
	std::string explanation;
	for (auto& magic : currentSelectPlayer->GetMagicList()) {
		color = (i == selectMagicIndex) ? 0xffff00 : 0xffffff;
		MagicData magicData = ElementDataManager::GetInstance().GetMagicData(magic);
		// 説明文だけもらう
		if (i == selectMagicIndex)
			explanation = magicData.explanation;

		int x = (offset.x + (stringSize * ((i + 1) % 2))) + (((offset2.x - offset.x) / 2) * (i % 2));
		int y = (offset.y + stringSize) + (stringSize * (i / 2));
		DrawFormatString(x, y, color, "%s", MyJson::Utf8ToString(magicData.name).c_str());
		i++;
	}

	int row = currentSelectPlayer->GetMaxMagics() / 2;

	int y = offset.y + (stringSize * (row + 2));

	// 選択中のアイテムの説明
	DrawLine(offset.x + stringSize, y, offset2.x - stringSize, y, 0xffffff);
	DrawString(offset.x + stringSize, y + stringSize, MyJson::Utf8ToString(explanation).c_str(), 0xffffff);

	// 操作のアシスト
	float ex = 0.8f;
	std::string assist = MyJson::Utf8ToString("決定：Enter/戻る：B");
	int stringNum = assist.length();
	int sx = offset2.x - (stringSize * ex) * (stringNum / 2) + (stringNum % 2);
	DrawExtendString(sx, offset2.y - (stringSize * ex), ex, ex, assist.c_str(), 0x808080);
}

bool FieldMagicCommand::Return() {

	selectMagicIndex = 0;

	return true;
}
