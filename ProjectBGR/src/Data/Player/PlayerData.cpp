#include "PlayerData.h"

PlayerData::PlayerData() {

}

PlayerData::~PlayerData() {
}

bool PlayerData::LevelUp() {
	if (!IsLevelUp()) return false;

	do {
		// 必要経験値を引いて余りは持ち越し
		currentExp -= levelUpNeedExp;
		// 次のレベルアップに必要な経験値を決める
		levelUpNeedExp *= 1.2f;
		// レベルを上げる
		level++;
		// ステータスポイントを増やす
		statusPoint += 4;
	} while (IsLevelUp());
	return true;
}
