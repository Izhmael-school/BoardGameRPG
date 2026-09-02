#include "MainGameScene.h"
#include "DxLib.h"
#include "GameObject/Camera/FreeCamera.h"
#include "Component/Collider/Collider.h"
#include "ImGui.h"

MainGameScene::MainGameScene()
	:gameObjectManager()
	, collisionManager() {
	Start();
}

MainGameScene::~MainGameScene() {
	delete resource;
}

void MainGameScene::Start() {
	gameObjectManager = std::make_unique<GameObjectManager>(resource);
	AABB aabb;
	aabb.halfSize = Vector3(Vector3::VScale(VOne,5000));
	aabb.center = VZero;
	collisionManager = std::make_unique<CollisionManager>(aabb);
}

/*
 *	準備処理
 */
void MainGameScene::Setup() {
	auto g = gameObjectManager->UseObject<GameObject>("", VZero, VZero, VOne);
	auto r = gameObjectManager->UseObject<GameObject>("", VZero, VZero, VOne);
	auto c = gameObjectManager->UseObject<FreeCamera>("", VZero, VZero, VOne);
	CollisionLayer a = CollisionLayer::Default;
	ColliderShape sphere;
	sphere.type = Sphere;
	sphere.sphere.radius = 10;
	g->AddComponent<Collider>(collisionManager.get(), sphere, CollisionLayer::Default);
	r->AddComponent<Collider>(collisionManager.get(), sphere, CollisionLayer::Default);
	//gameObjectManager->UnuseObject(std::move(g));
	//gameObjectManager->UnuseObject(std::move(r));
	//gameObjectManager->UnuseObject(std::move(c));
}

/*
 *	更新処理
 */
void MainGameScene::Update(float _t) {
	gameObjectManager->Update(_t);
	collisionManager->Update(_t);

	ImGui::Begin("CameraTransform");
	ImGui::End();
}

void MainGameScene::Render() {
	gameObjectManager->Render();
	collisionManager->Render();
	DrawString(100, 100, "MainGame", 0x000000);
#if _DEBUG 線

	// オブジェクトの位置関係がわかるように地面にラインを描画する
	{
		VECTOR pos1, pos2;

		// XZ平面 100.0f毎に1本ライン引き
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

		// X軸
		{
			pos1 = VGet(0, 0, 0);
			pos2 = VScale(VGet(1, 0, 0), 5000);	// VRight * 5000 をしてる
			DrawLine3D(pos1, pos2, 0xff0000);
		}

		// Y軸
		{
			pos1 = VGet(0, 0, 0);
			pos2 = VScale(VGet(0, 1, 0), 5000);	    // VUp * 5000 をしてる
			DrawLine3D(pos1, pos2, 0x00ff00);
		}

		// Z軸
		{
			pos1 = VGet(0, 0, 0);
			pos2 = VScale(VGet(0, 0, 1), 5000);	// VRight * 5000 をしてる
			DrawLine3D(pos1, pos2, 0x0000ff);
		}
	}

#endif
}

void MainGameScene::Cleanup() {

}