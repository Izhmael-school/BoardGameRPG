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
