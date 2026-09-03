#include "PlayerDataManager.h"
#include "Data/Player/PlayerData.h"

PlayerDataManager::PlayerDataManager()
{
}

PlayerDataManager::~PlayerDataManager()
{
}

void PlayerDataManager::CreatePlayer() {
	std::unique_ptr<PlayerData> newPlayerData = std::make_unique<PlayerData>();

	newPlayerData->SetMapPosition(Vector3(2, 3, 0));

	playerDataArray.push_back(std::move(newPlayerData));
}

PlayerData* PlayerDataManager::GetPlayerData(int _index){
	if (playerDataArray.size() <= _index) return nullptr;

	return playerDataArray[_index].get();
}

int PlayerDataManager::GetPlayerNum(){
	return playerDataArray.size();
}
