#include "UICanvasBase.h"
#include "../UIObject/UIObject.h"
#include "../UIObject/UIText/UIText.h"
#include "../UIObject/UIText/UITextDialog/UITextDialog.h"
#include "../UIObject/UISprite/UISprite.h"
#include "../UIObject/UIWindow/UIWindow.h"
#include "../UIObject/UISprite/UIRoulette/UIRoulette.h"
#include "../UIObject/UIBillboard/UIBillboard.h"
#include "Json/MyJson.h"
#include "Application.h"
#include "Manager/Resource/Sprite/SpriteResourceManager.h"

void UICanvasBase::Update(float _t, UIInput& _input) {
	for (auto& ui : uiArray) {
		ui->Update(_t, _input);
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

void UICanvasBase::AllDelete() {
	//std::erase_if(uiArray, [&](const UIPtr& p) {return true; });
	for (auto& ui : uiArray) {
		auto p = ui.get();

		DeleteUI(p);
	}
}

enum UIType {
	UIType_Object,
	UIType_Text,
	UIType_Sprite,
	UIType_Button,
	UIType_TextDialog,
	UIType_Window,
	UIType_Billboard,
	UIType_Roulette,
};


namespace {
	UITextStyle MakeTextStyle(json _data) {
		UITextStyle style;
		if (_data.contains("normalColor"))
			style.normalColor = MyJson::ParseColorCode(_data["normalColor"]);
		if (_data.contains("outlineColor"))
			style.outlineColor = MyJson::ParseColorCode(_data["outlineColor"]);
		if (_data.contains("selectedColor"))
			style.selectedColor = MyJson::ParseColorCode(_data["selectedColor"]);
		if (_data.contains("cantSelectColor"))
			style.cantSelectColor = MyJson::ParseColorCode(_data["cantSelectColor"]);
		if (_data.contains("fontName"))
			style.fontName = _data["fontName"];
		if (_data.contains("fontSize"))
			style.fontSize = _data["fontSize"];
		if (_data.contains("fontThickness"))
			style.fontThickness = _data["fontThickness"];
		if (_data.contains("edgeSize"))
			style.edgeSize = _data["edgeSize"];
		if (_data.contains("italic"))
			style.italic = _data["italic"];
		if (_data.contains("textDrawPosition"))
			style.textDrawPosition = _data["textDrawPosition"];
		if (_data.contains("fontType"))
			style.fontType = _data["fontType"];

		return style;
	}
}

void UICanvasBase::LoadCanvasForJson(const std::string& _filePath) {
	auto cd = MyJson::LoadJsonFile(_filePath);

	if (cd.empty()) return;

	for (auto& ui : cd["canvas"]) {
		if (!ui.contains("uiType")) continue;

		UIType type = ui["uiType"];
		// 座標の取得
		Vector2 pos = VZero_2;
		if (ui.contains("position")) {
			pos.x = ui["position"][0];
			pos.y = ui["position"][1];
		}
		// 表示の取得
		bool isActive = true;
		if (ui.contains("position"))
			isActive = ui["isActive"];
		// 名前の取得
		std::string name = "";
		if (ui.contains("name"))
			name = ui["name"];
		// 親の名前の取得
		std::string parentName = "";
		if (ui.contains("parentName"))
			parentName = ui["parentName"];

		UICreateMinimumNeedInfo info = { pos,name,parentName,isActive };

		switch (type) {
		case UIType_Object:
			CreateUIObject(info);
			break;
		case UIType_Text:
		{

			UITextStyle style = MakeTextStyle(ui);

			std::string text = "";
			if (ui.contains("text"))
				text = ui["text"];

			CreateUIText(info, text, style);
		}
		break;
		case UIType_Sprite:
		{
			float ex = 1.0f;
			if (ui.contains("ex"))
				ex = ui["ex"];
			float ey = 1.0f;
			if (ui.contains("ey"))
				ey = ui["ey"];
			float rot = 0.0f;
			if (ui.contains("rot"))
				rot = ui["rot"];
			std::string spriteName = "";
			if (ui.contains("spriteName"))
				spriteName = ui["spriteName"];

			CreateUISprite(info, spriteName, ex, ey, rot);
		}
		break;
		case UIType_Button:
			// 今の制作で使ってないため後回し
			break;
		case UIType_TextDialog:
		{
			UITextStyle style = MakeTextStyle(ui);

			CreateUITextDialog(info, style);
		}
		break;
		case UIType_Window:
		{
			unsigned int backColor = 0xffffff;
			if (ui.contains("backColor"))
				backColor = MyJson::ParseColorCode(ui["backColor"]);
			unsigned int frameColor = 0x000000;
			if (ui.contains("frameColor"))
				frameColor = MyJson::ParseColorCode(ui["frameColor"]);
			int w = 100;
			if (ui.contains("w"))
				w = ui["w"];
			int h = 100;
			if (ui.contains("h"))
				h = ui["h"];
			bool isCenter = false;
			if (ui.contains("isCenter"))
				isCenter = ui["isCenter"];
			int frameSize = 5;
			if (ui.contains("frameSize"))
				frameSize = ui["frameSize"];

			CreateUIWindow(info, backColor, frameColor, w, h, isCenter, frameSize);
		}
		break;
		case UIType_Billboard:
		{
			int spaceX = 0;
			if (ui.contains("spaceX"))
				spaceX = ui["spaceX"];
			int spaceY = 0;
			if (ui.contains("spaceY"))
				spaceY = ui["spaceY"];
			int col = 1;
			if (ui.contains("col"))
				col = ui["col"];
			int minCol = 0;
			if (ui.contains("minCol"))
				minCol = ui["minCol"];
			int maxCol = 9;
			if (ui.contains("maxCol"))
				maxCol = ui["maxCol"];
			bool isFitStringEnd = true;
			if (ui.contains("isFitStringEnd"))
				isFitStringEnd = ui["isFitStringEnd"];

			CreateUIBillboard(info, col, minCol, maxCol, spaceX, spaceY, isFitStringEnd);
		}
			break;
		case UIType_Roulette:
		{
			std::string rouletteName = "";
			if (ui.contains("rouletteName"))
				rouletteName = ui["rouletteName"];
			std::string arrowName = "";
			if (ui.contains("arrowName"))
				arrowName = ui["arrowName"];
			float ex = 1.0f;
			if (ui.contains("ex"))
				ex = ui["ex"];
			float ey = 1.0f;
			if (ui.contains("ey"))
				ey = ui["ey"];

			CreateUIRoulette(info, rouletteName, arrowName, ex, ey);
		}
			break;
		default:
			break;
		}
	}
}

void UICanvasBase::ReLoadCanvasForJson(const std::string& _filePath) {
	AllDelete();
	LoadCanvasForJson(_filePath);
}

UIObject* UICanvasBase::CreateUIObject(UICreateMinimumNeedInfo _info) {
	UIObject* ui = CreateUI<UIObject>(_info);

	if (!ui) return nullptr;

	return ui;
}

UIText* UICanvasBase::CreateUIText(UICreateMinimumNeedInfo _info, std::string _text, UITextStyle _style) {
	UIText* ui = CreateUI<UIText>(_info);

	if (!ui) return nullptr;
	ui->SetTextStyle(_style);
	ui->SetText(_text);

	return ui;
}

UISprite* UICanvasBase::CreateUISprite(UICreateMinimumNeedInfo _info, std::string _spriteName, float _ex, float _ey, float _rot) {
	UISprite* ui = CreateUI<UISprite>(_info);

	if (!ui) return nullptr;
	int graphHandle = Application::GetInstance().GetSpriteResourceManager()->GetSprite(_spriteName);
	ui->SetGraphHandle(graphHandle);
	ui->SetExtend(_ex, _ey);
	ui->SetAngle(_rot);

	return ui;
}

UITextDialog* UICanvasBase::CreateUITextDialog(UICreateMinimumNeedInfo _info, UITextStyle _style) {
	UITextDialog* ui = CreateUI<UITextDialog>(_info);

	if (!ui) return nullptr;
	ui->SetTextStyle(_style);

	return ui;
}

UIWindow* UICanvasBase::CreateUIWindow(UICreateMinimumNeedInfo _info, int _backColor, int _frameColor, int _w, int _h, bool _isCenter, int _frameSize) {
	UIWindow* ui = CreateUI<UIWindow>(_info);

	if (!ui) return nullptr;
	ui->SetBackColor(_backColor);
	ui->SetFrameColor(_frameColor);
	ui->SetWidth(_w);
	ui->SetHeight(_h);
	ui->SetIsCenter(_isCenter);
	ui->SetFrameSize(_frameSize);
	ui->Init();
	return ui;
}

UIBillboard* UICanvasBase::CreateUIBillboard(UICreateMinimumNeedInfo _info, int _col, int _minCol, int maxCol, int _spaceX, int _spaceY, bool _isFitStringEnd) {
	UIBillboard* ui = CreateUI<UIBillboard>(_info);

	if (!ui) return nullptr;
	ui->SetFitStringEnd(_isFitStringEnd);
	ui->SetColumn(_col);
	ui->SetSpace(_spaceX, _spaceY);
	ui->SetDrawRange(_minCol, maxCol);
	return ui;
}

UIRoulette* UICanvasBase::CreateUIRoulette(UICreateMinimumNeedInfo _info, std::string _rouletteName, std::string _arrowName, float _ex, float _ey) {
	UIRoulette* ui = CreateUI<UIRoulette>(_info);

	if (!ui) return nullptr;
	auto spriteResourceManager = Application::GetInstance().GetSpriteResourceManager();

	int rouletteHandle = spriteResourceManager->GetSprite(_rouletteName);
	ui->SetGraphHandle(rouletteHandle);
	int arrowHandle = spriteResourceManager->GetSprite(_arrowName);
	ui->SetArrowGraphHandle(arrowHandle);
	ui->SetExtend(_ex, _ey);
	ui->Init();

	return ui;
}