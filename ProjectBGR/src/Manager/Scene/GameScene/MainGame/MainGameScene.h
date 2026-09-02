/*
 *	@file	MainGameScene.h
 *  @author Sekino
 */

#ifndef _MAINGAMESCENE_H_
#define _MAINGAMESCENE_H_

#include "../SceneBase.h"
#include "Manager/GameObject/GameObjectManager.h"
#include "Manager/Resource/Model/ModelResourceManager.h" 
#include "Manager/Collision/CollisionManager.h" 
#include <memory>

class FreeCamera;

class MainGameScene :public SceneBase {
private:
	std::unique_ptr<FreeCamera> camera;

	std::unique_ptr<GameObjectManager> gameObjectManager;

	std::unique_ptr<CollisionManager> collisionManager;

	ModelResourceManager* resource;
public:
	MainGameScene();
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
