#include "GameObjectManager.h"

GameObjectManager::GameObjectManager(ModelResourceManager* _modelResourceManager)
	:generator() 
	,modelResourceManager(_modelResourceManager)
{
	generator = std::make_unique<GameObjectGenerator>(_modelResourceManager);
}

void GameObjectManager::UnuseObject(GameObjectPtr _gameObject) {
	// 蝙九°繧迂D繧剃ｽ懊ｋ
	std::type_index classID = _gameObject->GetClassID();
	// 蠕悟・逅・
	_gameObject->Cleanup();
	// 驟榊・縺ｫ蜈･繧後ｋ
	objectPool[classID].push_back(std::move(_gameObject));
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
