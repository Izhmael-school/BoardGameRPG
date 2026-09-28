#include "SceneManager.h"
#include "../Input/InputManager.h"
#include "../Fade/FadeManager.h"
#include "GameScene/Title/TitleScene.h"
#include "GameScene/MainGame/MainGameScene.h"
#include "GameScene/Debug/DebugSelectScene.h"
#include "Manager/UI/UIManager.h"
#include "UI/Input/UIInput.h"

SceneManager::~SceneManager() = default;

SceneManager::SceneManager() 
	: currentSceneType(SceneType::DebugSceneSelect)
	, nextSceneType(SceneType::Invalid)
	,isFade(false)
{ Start(); }

void SceneManager::Start(){

	// UIManagerを先に生成
	uiManager = std::make_unique<UIManager>();

	// 繧ｷ繝ｼ繝ｳ縺ｮ逕滓・
	scene[static_cast<int>(SceneType::DebugSceneSelect)] = std::make_unique<DebugSceneSelectScene>();
	scene[static_cast<int>(SceneType::Title)] = std::make_unique<TitleScene>();
	scene[static_cast<int>(SceneType::MainGame)] = std::make_unique<MainGameScene>(uiManager.get());

	// 蛻晄悄繧ｷ繝ｼ繝ｳ縺ｮ貅門ｙ
	scene[static_cast<int>(currentSceneType)]->Setup();
}

void SceneManager::Update(float _t){
	if (isFade) {

		FadeManager& fade = FadeManager::GetInstance();
		// 繝輔ぉ繝ｼ繝峨い繧ｦ繝医′邨ゅｏ縺｣縺溘ｉ
		if (fade.IsFadeOutEnd()) {
			// 蠕悟・逅・
			scene[currentSceneType]->Cleanup();
			// 蛻晄悄蛹・
			scene[nextSceneType]->Setup();
			currentSceneType = nextSceneType;
			nextSceneType = SceneType::Invalid;
			// 繝輔ぉ繝ｼ繝峨う繝ｳ
			fade.FadeStart(FadeIn, FadeNormal);
			return;
		}

		if (fade.IsFadeInEnd()) {
			isFade = false;
			return;
		}
	}

	UIInput uiInput;
	InputManager::GetInstance().UpdateUIInput(uiInput);

	scene[static_cast<int>(currentSceneType)]->Update(_t);
	uiManager->Update(_t,uiInput);

#if _DEBUG
	if (InputManager::GetInstance().IsKeyDown(KEY_BACK))
		ChangeScene(SceneType::DebugSceneSelect);
#endif
}

void SceneManager::Render(){
	scene[static_cast<int>(currentSceneType)]->Render();
	uiManager->Render();
}

void SceneManager::ChangeScene(SceneType _nextSceneType){
	if(currentSceneType == _nextSceneType || _nextSceneType == SceneType::Invalid) return;
	// 繝輔ぉ繝ｼ繝峨↓蜈･繧・
	FadeManager::GetInstance().FadeStart(FadeOut, FadeNormal);
	isFade = true;
	nextSceneType = _nextSceneType;
}
