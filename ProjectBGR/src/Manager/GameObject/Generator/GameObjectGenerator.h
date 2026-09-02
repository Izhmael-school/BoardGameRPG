/*
 * @brief インスタンスを持ったオブジェクトを生成する
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
	 * @brief GameObjectを継承したオブジェクトの生成
	 */
	template<typename T, typename... Args>
	std::unique_ptr<T> CreateGameObject(std::string _modelName = "", Args&&... args);
};

#include "Manager/Resource/Model/ModelResourceManager.h" 
template<typename T, typename... Args>
inline std::unique_ptr<T> GameObjectGenerator::CreateGameObject(std::string _modelName,Args&&... args ) {
	// GameObjectを継承していなければ帰る
	if (!std::is_base_of<GameObject, T>::value)
		return nullptr;

	int modelHandle = -1;

	if (!_modelName.empty()) {
		// モデルハンドルの取得
		modelHandle = modelResourceManager.GetDupModelHandle(_modelName);
	}

	// 生成
	std::unique_ptr<T> object = std::make_unique<T>(modelHandle,std::forward<Args>(args)...);

	return std::move(object);
}
#endif