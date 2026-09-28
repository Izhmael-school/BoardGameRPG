/*
 *	@file	MainGameScene.h
 *  @author Sekino
 */

#pragma once
#ifndef _MAINGAMESCENE_H_
#define _MAINGAMESCENE_H_

#include "../SceneBase.h"
#include <memory>

class FreeCamera;
class GameObjectManager;
class CollisionManager;
class MapManager;
class TurnProcessor;
class CharacterDataManager;
class ModelResourceManager;
class UIManager;

class MainGameScene :public SceneBase {
private:
	std::unique_ptr<FreeCamera> camera;

	std::unique_ptr<GameObjectManager> gameObjectManager;

	std::unique_ptr<CollisionManager> collisionManager;

	std::unique_ptr<MapManager> mapManager;

	std::unique_ptr<TurnProcessor> turn;

	std::unique_ptr<CharacterDataManager> characterDataManager;

	ModelResourceManager* resource;

	UIManager* uiManager;
public:
	MainGameScene(UIManager* _uiManager);
	~MainGameScene();

private:
	void Start() override;

public:
	void Setup() override;

	void Update(float _t) override;

	void Render() override;

	void Cleanup() override;

};


#endif // !_MAINGAMESCENE_H_
