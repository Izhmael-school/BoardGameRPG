#include "SceneManager.h"
#include "../Input/InputManager.h"
#include "Definition/Const/KeyInputConst.h"
#include "../Fade/FadeManager.h"
#include "GameScene/Title/TitleScene.h"
#include "GameScene/MainGame/MainGameScene.h"
#include "GameScene/Debug/DebugSelectScene.h"

SceneManager::SceneManager() 
	: currentSceneType(SceneType::DebugSceneSelect)
	, nextSceneType(SceneType::Invalid)
	,isFade(false)
{ Start(); }

void SceneManager::Start(){

	// シーンの生成
	scene[static_cast<int>(SceneType::DebugSceneSelect)] = std::make_unique<DebugSceneSelectScene>();
	scene[static_cast<int>(SceneType::Title)] = std::make_unique<TitleScene>();
	scene[static_cast<int>(SceneType::MainGame)] = std::make_unique<MainGameScene>();

	// 初期シーンの準備
	scene[static_cast<int>(currentSceneType)]->Setup();
}

void SceneManager::Update(float _t){
	if (isFade) {

		FadeManager& fade = FadeManager::GetInstance();
		// フェードアウトが終わったら
		if (fade.IsFadeOutEnd()) {
			// 後処理
			scene[currentSceneType]->Cleanup();
			// 初期化
			scene[nextSceneType]->Setup();
			currentSceneType = nextSceneType;
			nextSceneType = SceneType::Invalid;
			// フェードイン
			fade.FadeStart(FadeIn, FadeNormal);
			return;
		}

		if (fade.IsFadeInEnd()) {
			isFade = false;
			return;
		}
	}

	scene[static_cast<int>(currentSceneType)]->Update(_t);

#if _DEBUG
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_BACK))
		ChangeScene(SceneType::DebugSceneSelect);
#endif
}

void SceneManager::Render(){
	scene[static_cast<int>(currentSceneType)]->Render();
}

void SceneManager::ChangeScene(SceneType _nextSceneType){
	if(currentSceneType == _nextSceneType || _nextSceneType == SceneType::Invalid) return;
	// フェードに入る
	FadeManager::GetInstance().FadeStart(FadeOut, FadeNormal);
	isFade = true;
	nextSceneType = _nextSceneType;
}
