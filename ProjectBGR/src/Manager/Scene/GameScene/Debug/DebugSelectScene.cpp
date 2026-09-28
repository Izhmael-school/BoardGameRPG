#include "../Debug/DebugSelectScene.h"
#include "../../SceneManager.h"
#include "Manager/Input/InputManager.h"
#include "Definition/Const/KeyInputConst.h"
#include <algorithm>
#include <math.h>
#include "DxLib.h"

DebugSceneSelectScene::DebugSceneSelectScene()
	:currentScene(0)
{
	Start();
}

DebugSceneSelectScene::~DebugSceneSelectScene()
{
}

void DebugSceneSelectScene::Start() {

	sceneInfoArray.push_back({ "Title",[]() {SceneManager::GetInstance().ChangeScene(SceneType::Title);} });
	sceneInfoArray.push_back({ "MainGame",[]() {SceneManager::GetInstance().ChangeScene(SceneType::MainGame);} });
}

void DebugSceneSelectScene::Update(float _t) {
	int size = static_cast<int>(sceneInfoArray.size());

	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_UP))
		currentScene = max(currentScene - 1, 0);
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_DOWN))
		currentScene = min(currentScene + 1, size - 1);
	if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_RETURN))
		sceneInfoArray[currentScene].SceneChangeFunc();
}

void DebugSceneSelectScene::Render() {
	int size = static_cast<int>(sceneInfoArray.size());
	for (int i = 0;i < size;i++) {
		if (i == currentScene)
			DrawString(100, 100 + (20 * i), sceneInfoArray[i].sceneName.c_str(), 0xffff00);
		else
			DrawString(100, 100 + (20 * i), sceneInfoArray[i].sceneName.c_str(), 0x000000);
	}
}

void DebugSceneSelectScene::Setup() {
	currentScene = 0;
}
