#include "UICanvasBase.h"
#include "../UIObject/UIObject.h"

void UICanvasBase::Update(float _t, UIInput& _input) {
	for (auto& ui : uiArray) {
		ui->Update(_t,_input);
	}
}

void UICanvasBase::Render() {
	for (auto& ui : uiArray) {
		if (!ui->IsActive()) continue;
		ui->Render();
	}
}

void UICanvasBase::DeleteUI(UIObject*& _ui) {
	auto itr = std::find_if(uiArray.begin(), uiArray.end(),
		[&](const auto& p) { return p.get() == _ui; });
	if (itr == uiArray.end())return;
	(*itr).reset();
	uiArray.erase(itr);
	_ui = nullptr;
}
