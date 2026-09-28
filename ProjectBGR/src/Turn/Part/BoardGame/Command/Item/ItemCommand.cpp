#include "ItemCommand.h"
#include "Manager/Input/InputManager.h"
#include "Data/Player/PlayerData.h"
#include "Manager/Data/ElementDataManager.h"
#include "DxLib.h"

bool ItemCommand::SelectItem() {
	// データがないかこのターンすでにアイテムを使っていたら帰る
	if (!currentSelectPlayer || !currentSelectPlayer->IsUseItem()) return Return();

	int itemNum = static_cast<int>(currentSelectPlayer->GetItemList().size()) - 1;

	InputManager& input = InputManager::GetInstance();

	if (input.IsKeyDown(KEY_INPUT_UP))
		selectItemIndex = max(0, selectItemIndex - 2);
	if (input.IsKeyDown(KEY_INPUT_DOWN))
		selectItemIndex = min(itemNum, selectItemIndex + 2);
	if (input.IsKeyDown(KEY_INPUT_RIGHT))
		selectItemIndex = min(itemNum,(selectItemIndex % 2 == 0) ? selectItemIndex + 1 : selectItemIndex);
	if (input.IsKeyDown(KEY_INPUT_LEFT))
		selectItemIndex = (selectItemIndex % 2 == 1) ? selectItemIndex - 1 : selectItemIndex;

	if (input.IsKeyDown(KEY_INPUT_B))
		return Return();

	if (input.IsKeyDown(KEY_INPUT_RETURN)) {
		// アイテムを使う
		UseItem(currentSelectPlayer->GetItemList()[selectItemIndex]);
		return Return();
	}
	return false;
}

void ItemCommand::UseItem(int _effectID) {
	// 繧｢繧､繝・Β繧剃ｽｿ縺｣縺溷屓謨ｰ繧貞｢励ｄ縺・
	currentSelectPlayer->AddUsedItemCount();

	// 繧｢繧､繝・Β繧呈ｶ医☆
	currentSelectPlayer->RemoveItem(selectItemIndex);
}

void ItemCommand::Render() {
	//VECTOR offset = VGet(400, 200, 0);
	//VECTOR offset2 = VGet(700, 400, 0);
	//int stringSize = 20;
	//// 繧ｦ繧｣繝ｳ繝峨え
	//DrawFillBox(offset.x, offset.y, offset2.x, offset2.y, 0x000000);
	//DrawLineBox(offset.x, offset.y, offset2.x, offset2.y, 0xffffff);

	//// 繧｢繧､繝・Β蜷・
	//int i = 0;
	//unsigned int color = 0xffffff;
	//std::string explanation;
	//for (auto& item : currentSelectPlayer->GetItemList()) {
	//	color = (i == selectItemIndex) ? 0xffff00 : 0xffffff;
	//	ItemData itemData = ElementDataManager::GetInstance().GetItemData(item);
	//	// 隱ｬ譏取枚縺縺代ｂ繧峨≧
	//	if (i == selectItemIndex)
	//		explanation = itemData.explanation;

	//	int x = (offset.x + (stringSize * ((i + 1) % 2))) + (((offset2.x - offset.x) / 2) * (i % 2));
	//	int y = (offset.y + stringSize) + (stringSize * (i / 2));
	//	DrawFormatString(x, y, color, "%s",itemData.name.c_str());
	//	i++;
	//}

	//int row = currentSelectPlayer->GetMaxItems() / 2;

	//int y = offset.y + (stringSize * (row + 2));

	//// 驕ｸ謚樔ｸｭ縺ｮ繧｢繧､繝・Β縺ｮ隱ｬ譏・
	//DrawLine(offset.x + stringSize, y, offset2.x - stringSize, y, 0xffffff);
	//DrawString(offset.x + stringSize, y + stringSize, explanation.c_str(), 0xffffff);

	//// 謫堺ｽ懊・繧｢繧ｷ繧ｹ繝・
	//float ex = 0.8f;
	//std::string assist = "決定:Enter/戻る:B";
	//int stringNum = assist.length();
	//int sx = offset2.x - (stringSize * ex) * (stringNum / 2) + (stringNum % 2);
	//DrawExtendString(sx, offset2.y - (stringSize * ex), ex, ex, assist.c_str(), 0x808080);
}

bool ItemCommand::Return() {

	selectItemIndex = 0;

	return true;
}
