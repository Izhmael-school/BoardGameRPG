/*
 * @brief GameObject繧堤ｶ呎価縺励◆繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ邂｡逅・☆繧九け繝ｩ繧ｹ
 * @author Sekino
 */
#pragma once
#ifndef _GAMEOBJECTMANAGER_H_
#define _GAMEOBJECTMANAGER_H_

#include "Generator/GameObjectGenerator.h"
#include "GameObject/GameObject.h"
#include "../ManagerBase.h"
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <string>

class ModelResourceManager;

using GameObjectPtr = std::unique_ptr<GameObject>;
using GameObjectPtrArray = std::vector<GameObjectPtr>;
using GameObjectPool = std::unordered_map<std::type_index, GameObjectPtrArray>;

class GameObjectManager : public ManagerBase {
	std::unique_ptr<GameObjectGenerator> generator;	// 逕滓・

	ModelResourceManager* modelResourceManager;		// 繝｢繝・Ν邂｡逅・

	GameObjectPtrArray useObject;

	GameObjectPool objectPool;
public:
	GameObjectManager(ModelResourceManager* _modelResourceManager);

public:
	template<typename T, typename... Args>
	std::unique_ptr<T> Instantiate(std::string _modelName = "", Args&&... args);

	template<typename T, typename... Args>
	T* UseObject(std::string _modelName ,Vector3 _pos,Vector3 _rot,Vector3 _scale,Args&&... args);

	void UnuseObject(GameObjectPtr _gameObject);

	/*
	 * @brief 名前指定でオブジェクトを探す
	 */
	GameObject* Find(const std::string& _name);

	void Update(float _t);

	void Render();
};

template<typename T, typename ...Args>
inline std::unique_ptr<T> GameObjectManager::Instantiate( std::string _modelName,Args && ...args) {
	std::unique_ptr<T> gameObject = generator->CreateGameObject<T>( _modelName,std::forward<Args>(args)... );
	if (gameObject == nullptr) return nullptr;
	return std::move(gameObject);
}

template<typename T, typename ...Args>
inline T* GameObjectManager::UseObject(std::string _modelName, Vector3 _pos, Vector3 _rot, Vector3 _scale, Args && ...args) {
	// GameObject繧堤ｶ呎価縺励※縺ｪ縺代ｌ縺ｰ蟶ｰ繧・
	if (!std::is_base_of<GameObject, T>::value)
		return nullptr;
	// 繧ｯ繝ｩ繧ｹ縺ｮ逡ｪ蜿ｷ繧貞叙蠕・
	std::type_index classID = typeid(T);

	GameObjectPtrArray* pool = &objectPool[classID];
	GameObjectPtr object;

	// 縺ｪ縺代ｌ縺ｰ逕滓・縺ゅｌ縺ｰ蜀堺ｽｿ逕ｨ
	if (pool->empty())
		object = Instantiate<T>(_modelName, std::forward<Args>(args)...);
	else {
		object = std::move(pool->back());
		object->SetModelHandle(modelResourceManager->GetDupModelHandle(_modelName));
	}

	Transform* transform = object->GetTransform();

	transform->SetPosition(_pos);
	transform->SetRotation(_rot);
	transform->SetScale(_scale);

	object->Setup();

	useObject.emplace_back(std::move(object));

	return static_cast<T*>(useObject.back().get());
}

#endif
