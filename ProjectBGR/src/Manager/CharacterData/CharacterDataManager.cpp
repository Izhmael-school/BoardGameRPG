#include "CharacterDataManager.h"
#include "Definition/CommonModule/Json/MyJson.h"
#include "Data/Player/PlayerData.h"
#include "Data/Enemy/EnemyData.h"
#include "Manager/Map/MapManager.h"

CharacterDataManager::CharacterDataManager() {
}

CharacterDataManager::~CharacterDataManager() {
}

PlayerData* CharacterDataManager::CreatePlayerData(Job _job) {
	auto jobData = MyJson::LoadJsonFile("res/ExternalFile/Data/OriginPlayerData.json");
	std::unique_ptr<PlayerData> newPlayerData = std::make_unique<PlayerData>();
	// ステータスのセット
	for (auto& data : jobData) {
		if (data["jobID"] != _job) continue;
		newPlayerData->SetAtk(data["atk"]);
		newPlayerData->SetDef(data["def"]);
		newPlayerData->SetMag(data["mag"]);
		newPlayerData->SetSpd(data["spd"]);
		newPlayerData->SetLuk(data["luk"]);
		newPlayerData->SetVit(data["vit"]);
	}

	// 最大体力の更新
	newPlayerData->SetMaxHp();
	// 最大値の更新に合わせて体力を回復
	newPlayerData->Heal(newPlayerData->GetMaxHp());
	// 初期スタート地点にセット
	newPlayerData->SetMapPosition(3, 9);

	playerDataArray.emplace_back(std::move(newPlayerData));
	return playerDataArray.back().get();
}

std::vector<CharacterData*> CharacterDataManager::GetCharacterDataToMapPos(int _x, int _y) {
	std::vector<CharacterData*> characterList;

	// プレイヤーの捜索
	for (auto& p : playerDataArray) {
		Vector3 pos = p->GetMapPosition();
		if (pos.x == _x && pos.y == _y)
			characterList.emplace_back(p.get());
	}
	// 敵の捜索
	for (auto& e : enemyDataArray) {
		Vector3 pos = e->GetMapPosition();
		if (pos.x == _x && pos.y == _y)
			characterList.emplace_back(e.get());
	}

	return characterList;
}

EnemyData* CharacterDataManager::CreateEnemyData(int _enemyID, int _x, int _y, bool _isBoss) {
	auto jobenemyData = MyJson::LoadJsonFile("res/ExternalFile/Data/EnemyData.json");
	std::unique_ptr<EnemyData> enemyData = std::make_unique<EnemyData>();
	// ステータスのセット
	for (auto& data : jobenemyData) {
		if (data["enemyID"] != _enemyID) continue;
		enemyData->SetAtk(data["atk"]);
		enemyData->SetDef(data["def"]);
		enemyData->SetMag(data["mag"]);
		enemyData->SetSpd(data["spd"]);
		enemyData->SetLuk(data["luk"]);
		enemyData->SetVit(data["vit"]);
		enemyData->SetPlayerName(data["enemyName"]);
		enemyData->SetMoney(data["money"]);
		enemyData->SetCurrentExp(data["exp"]);
	}

	// 最大体力の更新
	enemyData->SetMaxHp();
	// 最大値の更新に合わせて体力を回復
	enemyData->Heal(enemyData->GetMaxHp());
	// スポーン地点をセット
	enemyData->SetMapPosition(_x, _y);

	enemyData->SetBoss(_isBoss);

	enemyDataArray.emplace_back(std::move(enemyData));
	return enemyDataArray.back().get();
}

void CharacterDataManager::DeleteEnemyData() {
	std::erase_if(enemyDataArray,
		[](std::unique_ptr<EnemyData>& enemyData) {
			if (enemyData->IsDelete()) {
				enemyData.reset();
				return true;
			}
			return false;
		});
}

void CharacterDataManager::DebugCreatePlayerData(Job _p1, Job _p2, Job _p3, Job _p4) {
	auto p1 = CreatePlayerData(_p1);
	p1->SetColor(0x0000ff);
	p1->AddItem(0);
	p1->AddItem(0);
	p1->AddItem(0);
	p1->AddMagic(0);
	auto p2 = CreatePlayerData(_p2);
	p2->SetColor(0xff0000);
	p2->AddItem(0);
	p2->AddMagic(0);
	auto p3 = CreatePlayerData(_p3);
	p3->SetColor(0xffff00);
	p3->AddItem(0);
	p3->AddMagic(0);
	auto p4 = CreatePlayerData(_p4);
	p4->SetColor(0x008000);
	p4->AddItem(0);
	p4->AddMagic(0);
}

#include "DxLib.h"
void CharacterDataManager::DebugRender() {
	int playerNum = GetPlayerNum();
	for (int i = 0; i < playerNum; i++) {
		PlayerData* data = GetPlayerData(i);
		Vector3 mapPos = data->GetMapPosition();
		int x = mapPos.x;
		int y = mapPos.y;

		DrawCircle(x * 32 + 16, y * 32 + 16, 10, data->GetColor());
	}

	int enemyNum = GetEnemyNum();
	for (int i = 0; i < enemyNum; i++) {
		EnemyData* data = GetEnemyData(i);
		Vector3 mapPos = data->GetMapPosition();
		int x = mapPos.x;
		int y = mapPos.y;

		DrawCircle(x * 32 + 16, y * 32 + 16, 10, 0x000000);
	}
}
