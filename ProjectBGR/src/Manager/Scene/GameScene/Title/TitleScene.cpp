#include "TitleScene.h"
#include <DxLib.h>

TitleScene::TitleScene(){
	Start();
}

TitleScene::~TitleScene() {
}

void TitleScene::Start() {
}

void TitleScene::Setup() {
	
}

void TitleScene::Update(float _t) {

}

void TitleScene::Render() {
	DrawString(940, 600, "Start", 0xffff00);
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
			pos1 = VGet(0,0,0);
			pos2 = VScale(VGet(1,0,0), 5000);	// VRight * 5000 をしてる
			DrawLine3D(pos1, pos2, 0xff0000);
		}

		// Y軸
		{
			pos1 = VGet(0,0,0);
			pos2 = VScale(VGet(0,1,0), 5000);	    // VUp * 5000 をしてる
			DrawLine3D(pos1, pos2, 0x00ff00);
		}

		// Z軸
		{
			pos1 = VGet(0,0,0);
			pos2 = VScale(VGet(0,0,1), 5000);	// VRight * 5000 をしてる
			DrawLine3D(pos1, pos2, 0x0000ff);
		}
	}
#endif
}

void TitleScene::Cleanup() {

}