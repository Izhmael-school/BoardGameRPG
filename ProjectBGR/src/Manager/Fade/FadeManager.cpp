#include "FadeManager.h"

FadeManager::FadeManager() 
{ Start(); }


void FadeManager::Start() {
	fade[FadeNormal] = std::make_unique<FadeBase>();
}

void FadeManager::Update(float _t) {
	// 繝輔ぉ繝ｼ繝峨′辟｡縺代ｌ縺ｰ蟶ｰ繧・
	if (!currentFade) return;

	// 繝輔ぉ繝ｼ繝峨・譖ｴ譁ｰ
	currentFade->Update(_t);

	prevFadeState = currentFadeState;
	currentFadeState = currentFade->GetCurrentState();

	// 繝輔ぉ繝ｼ繝峨う繝ｳ縺檎ｵゅｏ縺｣縺溘ｉ邨ゆｺ・ｮ｣險
	if (currentFade->GetCurrentState() == FadeNone)
		FadeEnd();
}

void FadeManager::Render() {
	if (!currentFade) return;

	currentFade->Render();
}

void FadeManager::FadeStart(FadeState _state, FadeType _type, float _time) {
	if (_state == FadeNone || _type == FadeMax || _time <= 0) return;

	time = _time;
	currentFadeType = _type;
	currentFadeState = _state;
	currentFade = fade[currentFadeType].get();
	currentFade->FadeStart(_state, _time);
}

void FadeManager::FadeEnd() {
	currentFadeState = FadeNone;
	currentFade = nullptr;
}
