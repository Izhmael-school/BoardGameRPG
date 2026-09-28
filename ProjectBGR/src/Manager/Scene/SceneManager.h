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

class UIManager;

class SceneManager : public ManagerBase, public Singleton<SceneManager>{
private:
	std::unique_ptr<SceneBase> scene[static_cast<int>(SceneType::Max)];
	std::unique_ptr<UIManager> uiManager;
	SceneType currentSceneType;
	SceneType nextSceneType;
	bool isFade;

public:
	SceneManager();
	~SceneManager();

private:
	void Start() override;

public:
	void Update(float _t) override;
	void Render() override;

	/// <summary>
	/// 繧ｷ繝ｼ繝ｳ縺ｮ螟画峩
	/// </summary>
	/// <param name="nextSceneType"></param>
	void ChangeScene(SceneType _nextSceneType);

	// 繧ｷ繝ｼ繝ｳ縺ｮ蜿門ｾ・
	inline SceneType GetCurrentSceneType() const { return currentSceneType; }
};

