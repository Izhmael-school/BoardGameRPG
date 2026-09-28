#include "MainGameScene.h"
#include "DxLib.h"
#include "GameObject/Camera/FreeCamera.h"
#include "Component/Collider/Collider.h"
#include "Manager/GameObject/GameObjectManager.h"
#include "Manager/Resource/Model/ModelResourceManager.h" 
#include "Manager/Collision/CollisionManager.h" 
#include "Manager/Map/MapManager.h"
#include "Turn/TurnProcessor.h"
#include "Manager/CharacterData/CharacterDataManager.h"
#include <memory>
#include "Data/Player/PlayerData.h"
#include "ImGui.h"
#include "Application.h"
#include "GameObject/Character/Character.h"
#include "UI/Canvas/SceneCanvas/MainGame/MainGameCanvas.h"
#include "Manager/UI/UIManager.h"

MainGameScene::MainGameScene(UIManager* _uiManager) 
	:uiManager(_uiManager)
{
	Start();
}

MainGameScene::~MainGameScene() {
	delete resource;
}

void MainGameScene::Start() {
	gameObjectManager = std::make_unique<GameObjectManager>(Application::GetInstance().GetModelResourceManager());
	AABB aabb;
	aabb.halfSize = Vector3(Vector3::VScale(VOne, 5000));
	aabb.center = VZero;
	collisionManager = std::make_unique<CollisionManager>(aabb);
	mapManager = std::make_unique<MapManager>();
	characterDataManager = std::make_unique<CharacterDataManager>();
	turn = std::make_unique<TurnProcessor>(mapManager.get(), characterDataManager.get(), gameObjectManager.get(),uiManager);
	sceneCanvas = std::make_unique<MainGameCanvas>();
	sceneCanvas->Init();

	// キャンバスの登録
	uiManager->PushCanvas(sceneCanvas.get());
}

/*
 *	貅門ｙ蜃ｦ逅・
 */
void MainGameScene::Setup() {
	auto c = gameObjectManager->UseObject<FreeCamera>("", VZero, VZero, VOne, "Camera");

	// プレイヤーの生成
	characterDataManager->DebugCreatePlayerData(Job_Fighter, Job_Wizard, Job_Thief, Job_OrdinaryPeople);

	unsigned int colorSet[4] = { 0x0000ff,0xff0000,0x00ff00,0xffff00 };

	for (int i = 0; i < characterDataManager->GetPlayerNum(); i++) {
		PlayerData* data = characterDataManager->GetPlayerData(i);
		data->SetPlayerName("Player" + std::to_string(i + 1));
		data->SetColor(colorSet[i]);
		GameObject* p = gameObjectManager->UseObject<Character>("Player", VZero, VZero, VOne, data->GetPlayerName(), data);
		int r, g, b;
		GetColor2(colorSet[i], &r, &g, &b);
		// 一部マテリアルの色変更
		p->ChangeMaterialColor("Stand", r, g, b);
		// サイズを小さくする
		p->GetTransform()->SetScale(0.5f);
		// 一度座標を更新しておく
		p->Update(0);
	}
	turn->TurnStart();

	// ステージの生成
	gameObjectManager->UseObject<GameObject>("Stage", VZero, VZero, VOne, "BoardGameStage");
	// バトルステージの生成
	gameObjectManager->UseObject<GameObject>("BattleStage", Vector3(10000, 0, 10000), VZero, VOne, "BattleStage");
}

/*
 *	譖ｴ譁ｰ蜃ｦ逅・
 */
void MainGameScene::Update(float _t) {
	gameObjectManager->Update(_t);
	collisionManager->Update(_t);
	mapManager->Update(_t);
	turn->Update(_t);
}

void MainGameScene::Render() {
	gameObjectManager->Render();
	collisionManager->Render();
	mapManager->Render();
	turn->Render();
	DrawString(100, 100, "MainGame", 0x000000);
#if _DEBUG 邱・

	// 繧ｪ繝悶ず繧ｧ繧ｯ繝医・菴咲ｽｮ髢｢菫ゅ′繧上°繧九ｈ縺・↓蝨ｰ髱｢縺ｫ繝ｩ繧､繝ｳ繧呈緒逕ｻ縺吶ｋ
	{
		VECTOR pos1, pos2;

		// XZ蟷ｳ髱｢ 100.0f豈弱↓1譛ｬ繝ｩ繧､繝ｳ蠑輔″
		{
			pos1 = VGet(-5000.0f, 0, -5000.0f);
			pos2 = VGet(-5000.0f, 0, 5000.0f);

			for (int i = 0; i < 100; i++) {
				DrawLine3D(pos1, pos2, GetColor(100, 100, 100));

				pos1.x += 100.0f;
				pos2.x += 100.0f;
			}

			pos1 = VGet(-5000.0f, 0, -5000.0f);
			pos2 = VGet(5000.0f, 0, -5000.0f);
			for (int i = 0; i < 100; i++) {
				DrawLine3D(pos1, pos2, GetColor(100, 100, 100));

				pos1.z += 100.0f;
				pos2.z += 100.0f;
			}
		}

		// X霆ｸ
		{
			pos1 = VGet(0, 0, 0);
			pos2 = VScale(VGet(1, 0, 0), 5000);	// VRight * 5000 繧偵＠縺ｦ繧・
			DrawLine3D(pos1, pos2, 0xff0000);
		}

		// Y霆ｸ
		{
			pos1 = VGet(0, 0, 0);
			pos2 = VScale(VGet(0, 1, 0), 5000);	    // VUp * 5000 繧偵＠縺ｦ繧・
			DrawLine3D(pos1, pos2, 0x00ff00);
		}

		// Z霆ｸ
		{
			pos1 = VGet(0, 0, 0);
			pos2 = VScale(VGet(0, 0, 1), 5000);	// VRight * 5000 繧偵＠縺ｦ繧・
			DrawLine3D(pos1, pos2, 0x0000ff);
		}
	}

#endif
}

void MainGameScene::Cleanup() {
	uiManager->PopCanvas(sceneCanvas.get());
}