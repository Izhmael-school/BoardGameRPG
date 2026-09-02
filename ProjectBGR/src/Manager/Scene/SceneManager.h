/*
 * @file SceneManager.h
 * @author Sekino
 */
#pragma once
#include <memory>
#include "../ManagerBase.h"
#include "DesignPattern/Singleton/Singleton.h"
#include "GameScene/SceneBase.h"

enum SceneType {
	Invalid = -1,
	DebugSceneSelect,
	Title,
	MainGame,
	Max
};

class SceneManager : public ManagerBase, public Singleton<SceneManager>{
private:
	std::unique_ptr<SceneBase> scene[static_cast<int>(SceneType::Max)];
	SceneType currentSceneType;
	SceneType nextSceneType;
	bool isFade;

public:
	SceneManager();
	~SceneManager() = default;

private:
	void Start() override;

public:
	void Update(float _t) override;
	void Render() override;

	/// <summary>
	/// シーンの変更
	/// </summary>
	/// <param name="nextSceneType"></param>
	void ChangeScene(SceneType _nextSceneType);

	// シーンの取得
	inline SceneType GetCurrentSceneType() const { return currentSceneType; }
};

