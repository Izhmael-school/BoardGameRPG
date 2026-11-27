#include "GameObjectManager.h"

GameObjectManager::GameObjectManager(ModelResourceManager* _modelResourceManager)
	:generator() 
	,modelResourceManager(_modelResourceManager)
{
	generator = std::make_unique<GameObjectGenerator>(_modelResourceManager);
}

void GameObjectManager::UnuseObject(GameObjectPtr _gameObject) {
	if (!_gameObject) return;
	// クラスの番号を取得
	std::type_index classID = _gameObject->GetClassID();
	// 使用後処理
	_gameObject->Cleanup();
	// クラス番号の配列に入れる
	objectPool[classID].emplace_back(std::move(_gameObject));
}

GameObject* GameObjectManager::Find(const std::string& _name) {
	for (auto& obj : useObject) {
		if (obj->GetName() != _name) continue;

		return obj.get();
	}
	return nullptr;
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
