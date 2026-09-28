#include "FadeBase.h"
#include "DxLib.h"

void FadeBase::Update(float _t) {
	if (currentState == FadeNone)return;

	// 繧｢繝ｫ繝輔ぃ縺ｮ蠅玲ｸ・
	alpha += (BLEND_MAX * _t / time) * static_cast<int>(currentState);

	switch (currentState) {
	case FadeIn:
		if (alpha > 0.0f) break;
		alpha = 0;
		currentState = FadeNone;
		break;
	case FadeOut:
		if (alpha < BLEND_MAX) break;
		alpha = BLEND_MAX;
		currentState = FadeNone;
		break;
	}
}

void FadeBase::Render() {

	// 謠冗判繧偵い繝ｫ繝輔ぃ繝｢繝ｼ繝峨↓縺吶ｋ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha);
	DrawFillBox(0, 0, 1920, 1080, color);
	// 謌ｻ縺・
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, -1);
}

void FadeBase::FadeStart(FadeState _state, float _time, int _color) {
	if (_state == FadeNone || currentState != FadeNone) return;
	currentState = _state;
	time = _time;
	alpha = currentState == FadeIn ? BLEND_MAX : 0.0f;
	color = _color;
}