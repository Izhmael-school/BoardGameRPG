/*
 * @brief UIを更新描画するキャンバス
 * @author Sekino
 */
#pragma once
#ifndef _UICANVAS_H_
#define _UICANVAS_H_

#include <memory>
#include <vector>
#include <string>
#include <type_traits>
#include "../UIContext.h"
#include "../UIObject/UIText/Style/UITextStyle.h"
#include "String/MyString.h"
#include "UICreateNeedInfo.h"
#include "Vector2.h"

class UIObject;
class UIText;
class UISprite;
class UITextDialog;
class UIWindow;
class UIBillboard;
class UIRoulette;
struct UIInput;

using UIPtr = std::unique_ptr<UIObject>;

const char* const UI_FOLDER_PATH = "res/ExternalFile/UI/";
const char* const JSON_EXTENSION = ".json";

class UICanvasBase {
protected:
	std::vector<UIPtr> uiArray;
	std::weak_ptr<UIContext> context;

public:
	UICanvasBase() = default;
	virtual ~UICanvasBase() = default;

	virtual void Init() {};
	virtual void Update(float _t, UIInput& _input);
	virtual void Render();
	virtual void Setup() {};

	void DeleteUI(UIObject*& _ui);

	virtual void AllDelete();

	/*
	 * @brief UIオブジェクトの生成
	 */
	template<class T, class... Args>
	T* Instantiate(Args&&... _args);

	template<class T, class... Args>
	std::unique_ptr<T> UniqueInstantiate(Args&&... _args);

	void SetUIContext(std::shared_ptr<UIContext> _context) { context = _context; }

	std::shared_ptr<UIContext> GetUIContext() { return context.lock(); }

public:
	void LoadCanvasForJson(const std::string& _filePath);

	void ReLoadCanvasForJson(const std::string& _filePath);
	template<class T>
	T* Find(const std::string& _uiName);

private:
	template<class T>
	T* CreateUI(UICreateMinimumNeedInfo _info);

	UIObject* CreateUIObject(UICreateMinimumNeedInfo _info);

	UIText* CreateUIText(UICreateMinimumNeedInfo _info, std::string _text, UITextStyle _style);

	UISprite* CreateUISprite(UICreateMinimumNeedInfo _info, std::string _spriteName, float _ex, float _ey, float _rot);

	UITextDialog* CreateUITextDialog(UICreateMinimumNeedInfo _info, UITextStyle _style);

	UIWindow* CreateUIWindow(UICreateMinimumNeedInfo _info, int _backColor, int _frameColor, int _w, int _h, bool _isCenter, int _frameSize);

	UIBillboard* CreateUIBillboard(UICreateMinimumNeedInfo _info, int _col, int _minCol,int maxCol,int _spaceX, int _spaceY, bool _isFitStringEnd);

	UIRoulette* CreateUIRoulette(UICreateMinimumNeedInfo _info, std::string _rouletteName, std::string _arrowName, float _ex, float _ey);
};


#include "../UIObject/UIObject.h"
template<class T, class ...Args>
inline T* UICanvasBase::Instantiate(Args && ..._args) {
	// Tの基底がUIObjectじゃないなら帰る
	if (!std::is_base_of<UIObject, T>::value)
		return nullptr;

	auto ui = std::make_unique<T>(std::forward<Args>(_args)...);

	ui->Init();

	T* ptr = ui.get();

	uiArray.emplace_back(std::move(ui));

	return ptr;
}

template<class T, class ...Args>
inline std::unique_ptr<T> UICanvasBase::UniqueInstantiate(Args && ..._args) {
	// Tの基底がUIObjectじゃないなら帰る
	if (!std::is_base_of<UIObject, T>::value)
		return nullptr;

	auto ui = std::make_unique<T>(std::forward<Args>(_args)...);

	ui->Init();

	return ui;
}

template<class T>
inline T* UICanvasBase::Find(const std::string& _uiName) {
	// 型がUIObjectの派生でなければnull
	if (!std::is_base_of<UIObject, T>::value)
		return nullptr;

	for (auto& ui : uiArray) {
		if (ui->GetName() == _uiName)
			return static_cast<T*>(ui.get());

		UIObject* get = ui->GetChild(_uiName);

		if (!get) continue;

		return static_cast<T*>(get);
	}

	return nullptr;
}

template<class T>
inline T* UICanvasBase::CreateUI(UICreateMinimumNeedInfo _info)  {
	T* ui = nullptr;

	if (_info.parentName.empty()) {
		ui = Instantiate<T>(_info.name);
	}
	else {
		auto uiPtr = UniqueInstantiate<T>(_info.name);
		ui = uiPtr.get();
		UIObject* parent = Find<UIObject>(_info.parentName);
		if (parent)
			parent->AddChild(std::move(uiPtr));
		else
			uiArray.emplace_back(std::move(uiPtr));
	}

	UIObject* uio = static_cast<UIObject*>(ui);
	uio->SetPosition(_info.pos);
	uio->SetActive(_info.isActive);

	return ui;
}
#endif // !_UICANVAS_H_