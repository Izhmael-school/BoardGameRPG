#include "UIManager.h"
#include "UI/Canvas/UICanvasBase.h"
#include "UI/Input/UIInput.h"

void UIManager::Update(float _t,UIInput& _input) {
	if (canvases.empty() || !canvases.back()) return;

	// 最前のものだけを更新
	canvases.back()->Update(_t,_input);
}

void UIManager::Render() {
	if (canvases.empty()) return;

	for (auto& canvas : canvases) {
		if (!canvas) continue;

		canvas->Render();
	}
}

void UIManager::PushCanvas(UICanvasBase* _pScreen) {
	canvases.emplace_back(_pScreen);
}

void UIManager::PopCanvas() {
	if (canvases.empty()) return;

	canvases.pop_back();
}

void UIManager::PopCanvas(UICanvasBase* _pCanvas) {
	if (canvases.empty() || !_pCanvas) return;

	auto itr = std::find(canvases.begin(), canvases.end(), _pCanvas);

	if (itr == canvases.end()) return;

	canvases.erase(itr);
}
