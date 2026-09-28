/*
 * @brief 繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧呈戟縺｣縺溘が繝悶ず繧ｧ繧ｯ繝医ｒ逕滓・縺吶ｋ
 * @author Sekino
 */
#pragma once
#ifndef _GAMEOBJECTGENERATOR_H_
#define _GAMEOBJECTGENERATOR_H_

#include <map>
#include <string>
#include <memory>
#include "DxLib.h"
#include <cassert>
#include <type_traits>

class GameObject;
class ModelResourceManager;

class GameObjectGenerator {
private:
	ModelResourceManager& modelResourceManager;

public:
	GameObjectGenerator(ModelResourceManager* _modelResourceManager);
	~GameObjectGenerator() = default;

public:
	/*
	 * @brief GameObject繧堤ｶ呎価縺励◆繧ｪ繝悶ず繧ｧ繧ｯ繝医・逕滓・
	 */
	template<typename T, typename... Args>
	std::unique_ptr<T> CreateGameObject(std::string _modelName = "", Args&&... args);
};

#include "Manager/Resource/Model/ModelResourceManager.h" 
template<typename T, typename... Args>
inline std::unique_ptr<T> GameObjectGenerator::CreateGameObject(std::string _modelName,Args&&... args ) {
	// GameObject繧堤ｶ呎価縺励※縺・↑縺代ｌ縺ｰ蟶ｰ繧・
	if (!std::is_base_of<GameObject, T>::value)
		return nullptr;

	int modelHandle = -1;

	if (!_modelName.empty()) {
		// 繝｢繝・Ν繝上Φ繝峨Ν縺ｮ蜿門ｾ・
		modelHandle = modelResourceManager.GetDupModelHandle(_modelName);
	}

	// 逕滓・
	std::unique_ptr<T> object = std::make_unique<T>(modelHandle,std::forward<Args>(args)...);

	return std::move(object);
}
#endif