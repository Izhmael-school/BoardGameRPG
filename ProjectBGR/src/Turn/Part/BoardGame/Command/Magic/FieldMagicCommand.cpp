#include "FieldMagicCommand.h"
#include "Manager/Input/InputManager.h"
#include "Data/Player/PlayerData.h"
#include "Manager/Data/ElementDataManager.h"
#include "Manager/CharacterData/CharacterDataManager.h"
#include "../../Effect/Item/ItemEffect.h"
#include "DxLib.h"
#include "UI/UIContext.h"

void FieldMagicCommand::Init() {
	context = std::make_shared<UIContext>();
	context->Register<bool>("IsSelectMagic", &isSelectMagic);
	context->Register<bool>("IsSelectTarget", &isSelectTarget);
	context->Register<bool>("IsUsed", &isUsed);
	context->Register<int>("selectMagicIndex", &selectMagicIndex);
	context->Register<int>("selectTargetIndex", &selectTargetIndex);
	context->Register<std::vector<CharacterData*>>("targets", &possibleTargetArray);
	context->Register<PlayerData*>("currentTurnPlayer", &currentTurnPlayer);
	context->Register<std::string>("MagicName", &magicName);
}

bool FieldMagicCommand::SelectMagic() {
	// データがないかこのターンすでにアイテムを使っていれば帰る
	if (!currentTurnPlayer) return Return();

	// 確定したらtrueで返す
	if (isConfirm || !currentTurnPlayer->IsUseMagic())
		return true;

	if (!isGetPossibleTargets) {
		// 攻撃できるキャラクターを探す
		possibleTargetArray = characterDataManager->GetAllCharacterData();
		// 自身は除く
		auto itr = std::find(possibleTargetArray.begin(), possibleTargetArray.end(), currentTurnPlayer);
		possibleTargetArray.erase(itr);
		isGetPossibleTargets = true;
		target.clear();
		isConfirm = false;
		return false;
	}

	int magicNum = static_cast<int>(currentTurnPlayer->GetMagicList().size()) - 1;

	InputManager& input = InputManager::GetInstance();
	int port = currentTurnPlayer->GetControllerNum();

	if (isSelectMagic) {

		if (input.IsButtonDown(port, XINPUT_DPAD_UP))
			selectMagicIndex = max(0, selectMagicIndex - 2);
		if (input.IsButtonDown(port, XINPUT_DPAD_DOWN))
			selectMagicIndex = min(magicNum, selectMagicIndex + 2);
		if (input.IsButtonDown(port, XINPUT_DPAD_RIGHT))
			selectMagicIndex = min(magicNum, (selectMagicIndex % 2 == 0) ? selectMagicIndex + 1 : selectMagicIndex);
		if (input.IsButtonDown(port, XINPUT_DPAD_LEFT))
			selectMagicIndex = (selectMagicIndex % 2 == 1) ? selectMagicIndex - 1 : selectMagicIndex;

		if (input.IsButtonDown(port, XINPUT_B))
			return Return();

		if (input.IsButtonDown(port, XINPUT_A)) {

			isSelectMagic = false;
			isSelectTarget = true;
			return false;
		}
	}

	if (isSelectTarget)
		return SelectTarget();

	return false;
}

void FieldMagicCommand::UseMagic() {
	int magicID = currentTurnPlayer->GetMagicList()[selectMagicIndex];
	// アイテムを使う
	MagicData magic = ElementDataManager::GetInstance().GetMagicData(magicID);
	// 繧｢繧､繝・Β繧剃ｽｿ縺｣縺溷屓謨ｰ繧貞｢励ｄ縺・
	currentTurnPlayer->AddUsedMagicCount();

	ItemAndMagicEffect::EffectExecute(magic.effectID, magic.effectValue, 0, currentTurnPlayer,TargetSearch());

	magicName = magic.name;

	isSelectMagic = true;
	isSelectTarget = false;
	isUsed = true;
	isGetPossibleTargets = false;
}

bool FieldMagicCommand::SelectTarget() {

	InputManager* input = &InputManager::GetInstance();
	int portNum = currentTurnPlayer->GetControllerNum();
	if (input->IsButtonDown(portNum, XINPUT_DPAD_RIGHT))
		selectTargetIndex = min(possibleTargetArray.size(), selectTargetIndex + 1);
	if (input->IsButtonDown(portNum, XINPUT_DPAD_LEFT))
		selectTargetIndex = max(0, selectTargetIndex - 1);
	if (input->IsButtonDown(portNum, XINPUT_DPAD_UP))
		selectTargetIndex = max(0, selectTargetIndex - 4);
	if (input->IsButtonDown(portNum, XINPUT_DPAD_DOWN))
		selectTargetIndex = min(possibleTargetArray.size(), selectTargetIndex + 4);
	if (input->IsButtonDown(portNum, XINPUT_B)) {
		isSelectMagic = true;
		isSelectTarget = false;
	}
	if (input->IsButtonDown(portNum, XINPUT_A)) {
		isSelectMagic = true;
		isSelectTarget = false;
		if(!possibleTargetArray.empty())
		target.emplace_back(possibleTargetArray[selectTargetIndex]);
		possibleTargetArray.clear();
		isConfirm = true;
		return true;
	}
	return false;
}

void FieldMagicCommand::Render() {
	VECTOR offset = VGet(400, 200, 0);
	VECTOR offset2 = VGet(700, 400, 0);
	int stringSize = 20;
	// 繧ｦ繧｣繝ｳ繝峨え
	DrawFillBox(offset.x, offset.y, offset2.x, offset2.y, 0x000000);
	DrawLineBox(offset.x, offset.y, offset2.x, offset2.y, 0xffffff);

	// 繧｢繧､繝・Β蜷・
	int i = 0;
	unsigned int color = 0xffffff;
	std::string explanation;
	for (auto& magic : currentTurnPlayer->GetMagicList()) {
		color = (i == selectMagicIndex) ? 0xffff00 : 0xffffff;
		MagicData magicData = ElementDataManager::GetInstance().GetMagicData(magic);
		// 隱ｬ譏取枚縺縺代ｂ繧峨≧
		if (i == selectMagicIndex)
			explanation = magicData.explanation;

		int x = (offset.x + (stringSize * ((i + 1) % 2))) + (((offset2.x - offset.x) / 2) * (i % 2));
		int y = (offset.y + stringSize) + (stringSize * (i / 2));
		DrawFormatString(x, y, color, "%s", magicData.name.c_str());
		i++;
	}

	int row = currentTurnPlayer->GetMaxMagics() / 2;

	int y = offset.y + (stringSize * (row + 2));

	// 驕ｸ謚樔ｸｭ縺ｮ繧｢繧､繝・Β縺ｮ隱ｬ譏・
	DrawLine(offset.x + stringSize, y, offset2.x - stringSize, y, 0xffffff);
	DrawString(offset.x + stringSize, y + stringSize, explanation.c_str(), 0xffffff);

	// 謫堺ｽ懊・繧｢繧ｷ繧ｹ繝・
	float ex = 0.8f;
	std::string assist = "決定:Enter/戻る:B";
	int stringNum = assist.length();
	int sx = offset2.x - (stringSize * ex) * (stringNum / 2) + (stringNum % 2);
	DrawExtendString(sx, offset2.y - (stringSize * ex), ex, ex, assist.c_str(), 0x808080);
}

bool FieldMagicCommand::Return() {

	selectMagicIndex = 0;
	isGetPossibleTargets = false;
	isCancel = true;
	return true;
}

void FieldMagicCommand::RemoveUsedMagic() {
	// 使ったアイテムを消す
	currentTurnPlayer->RemoveMagic(selectMagicIndex);
	selectMagicIndex = 0;
	isConfirm = false;
	isUsed = false;
}

std::vector<CharacterData*> FieldMagicCommand::TargetSearch() {
	int magicID = currentTurnPlayer->GetMagicList()[selectMagicIndex];
	MagicData magic = ElementDataManager::GetInstance().GetMagicData(magicID);

	std::string ts = magic.target;

	if (ts == "All")
		return possibleTargetArray;
	else if (ts == "1sr")
		return characterDataManager->GetCharacterForOneSquareRadius(target[0]->GetMapPosition(), 1);
	
	return std::vector<CharacterData*>(target);
}
