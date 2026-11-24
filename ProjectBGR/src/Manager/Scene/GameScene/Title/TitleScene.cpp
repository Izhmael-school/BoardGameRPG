#include "TitleScene.h"
#include "UI/Canvas/SceneCanvas/Title/TitleCanvas.h"
#include "Manager/UI/UIManager.h"
#include "Manager/Input/InputManager.h"
#include <DxLib.h>

TitleScene::TitleScene(UIManager* _uiManager, std::function<void(SceneType)> _changeSceneFunc)
	:uiManager(_uiManager)
	,changeSceneFunc(_changeSceneFunc)
{
	Start();
}

TitleScene::~TitleScene() {
}

void TitleScene::Start() {
	sceneCanvas = std::make_unique<TitleCanvas>();
}

void TitleScene::Setup() {
	uiManager->PushCanvas(sceneCanvas.get());
	sceneCanvas->Setup();
}

void TitleScene::Update(float _t) {
	if (InputManager::GetInstance().IsButtonDown(XINPUT_A))
		if (changeSceneFunc)
			changeSceneFunc(PlayerSetting);
}

void TitleScene::Render() {
	DrawString(940, 600, "Start", 0xffff00);
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
			pos1 = VGet(0,0,0);
			pos2 = VScale(VGet(1,0,0), 5000);	// VRight * 5000 繧偵＠縺ｦ繧・
			DrawLine3D(pos1, pos2, 0xff0000);
		}

		// Y霆ｸ
		{
			pos1 = VGet(0,0,0);
			pos2 = VScale(VGet(0,1,0), 5000);	    // VUp * 5000 繧偵＠縺ｦ繧・
			DrawLine3D(pos1, pos2, 0x00ff00);
		}

		// Z霆ｸ
		{
			pos1 = VGet(0,0,0);
			pos2 = VScale(VGet(0,0,1), 5000);	// VRight * 5000 繧偵＠縺ｦ繧・
			DrawLine3D(pos1, pos2, 0x0000ff);
		}
	}
#endif
}

void TitleScene::Cleanup() {
	uiManager->PopCanvas(sceneCanvas.get());
}