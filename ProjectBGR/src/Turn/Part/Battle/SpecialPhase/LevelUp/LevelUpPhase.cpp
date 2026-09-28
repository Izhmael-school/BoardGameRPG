#include "LevelUpPhase.h"
#include "Data/Player/PlayerData.h"
#include "Manager/Input/InputManager.h"
#include "Definition/CommonModule/String/MyString.h"
#include <format>
#include "DxLib.h"

PlayerData* LevelUpPhase::levelUpPlayer = nullptr;
int LevelUpPhase::currentSelectStatus = 0;
int LevelUpPhase::upStatus[canUpStatus] = {0,0,0,0,0,0};
int LevelUpPhase::remainingStatusPoint = -1;

void LevelUpPhase::Render() {
	std::vector<std::string> statusText = { "ATK {} + {}","DEF {} + {}","Vit {} + {}","MAG {} + {}","SPD {} + {}","LUK {} + {}" };
	std::vector<int> status = { levelUpPlayer->GetAtk(),levelUpPlayer->GetDef(),levelUpPlayer->GetVit() ,levelUpPlayer->GetMag(), levelUpPlayer->GetSpd(), levelUpPlayer->GetLuk() };
	float xCenter = 1920 / 2;
	float yCenter = 1080 / 2;
	int index = -2;
	int charExtend = 2;
	int charSize = 18 * charExtend;
	for (auto t : statusText) {
		int x = xCenter;
		int y = yCenter + charSize * index;
		std::string s = std::vformat(t, std::make_format_args(status[index + 2], upStatus[index + 2]));
		MyString::StringCenterPos(s.c_str(), -1, &x, &y, charExtend, charExtend);
		DrawExtendString(x, y, charExtend, charExtend, s.c_str(), index + 2 == currentSelectStatus ? 0xffff00 : 0xffffff);
		index++;
	}
	std::string pointString = std::vformat("残り{}ポイント", std::make_format_args(remainingStatusPoint));
	int x = xCenter;
	int y = yCenter + charSize * index;
	MyString::StringCenterPos(pointString.c_str(), -1, &x, &y, charExtend, charExtend);
	DrawExtendString(x, y, charExtend, charExtend, pointString.c_str(), index + 2 == currentSelectStatus ? 0xffff00 : 0xffffff);
}

bool LevelUpPhase::LevelUp() {
	if (!levelUpPlayer) return false;

	if (remainingStatusPoint == -1)
		remainingStatusPoint = levelUpPlayer->GetStatusPoint();

	if (InputManager::GetInstance().IsKeyDown(KEY_UP))
		currentSelectStatus = max(0, currentSelectStatus - 1);
	if (InputManager::GetInstance().IsKeyDown(KEY_DOWN))
		currentSelectStatus = min(canUpStatus - 1, currentSelectStatus + 1);

	// ステータスを増やす
	if (remainingStatusPoint > 0 && InputManager::GetInstance().IsKeyDown(KEY_RIGHT)) {
		upStatus[currentSelectStatus]++;
		remainingStatusPoint--;
	}

	// ステータスを減らす
	if (InputManager::GetInstance().IsKeyDown(KEY_LEFT)) {
		if (upStatus[currentSelectStatus] > 0) {
			upStatus[currentSelectStatus]--;
			remainingStatusPoint++;
		}
	}

	// 確定
	if (InputManager::GetInstance().IsKeyDown(KEY_RETURN)) {

		levelUpPlayer->AddAtk(upStatus[0]);
		levelUpPlayer->AddDef(upStatus[1]);
		levelUpPlayer->AddVit(upStatus[2]);
		levelUpPlayer->AddMag(upStatus[3]);
		levelUpPlayer->AddSpd(upStatus[4]);
		levelUpPlayer->AddLuk(upStatus[5]);

		currentSelectStatus = 0;
		for (int i = 0; i < canUpStatus; i++) {
			upStatus[i] = 0;
		}
		remainingStatusPoint = -1;
		levelUpPlayer = nullptr;

		return true;
	}
	return false;
}