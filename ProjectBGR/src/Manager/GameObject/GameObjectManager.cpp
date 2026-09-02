#include "GameObjectManager.h"

GameObjectManager::GameObjectManager(ModelResourceManager* _modelResourceManager)
	:generator() 
	,modelResourceManager(_modelResourceManager)
{
	generator = std::make_unique<GameObjectGenerator>(_modelResourceManager);
}

void GameObjectManager::UnuseObject(GameObjectPtr _gameObject) {
	// 型からIDを作る
	std::type_index classID = _gameObject->GetClassID();
	// 後処理
	_gameObject->Cleanup();
	// 配列に入れる
	objectPool[classID].push_back(std::move(_gameObject));
}

void GameObjectManager::Update(float _t) {
	for (auto& object : useObject) {
		object->Update(_t);
	}
}

void GameObjectManager::Render() {
	for (auto& object : useObject) {
		object->Render();
	}
}
