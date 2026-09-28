/*
 * @brief UIを更新描画するキャンバス
 * @author Sekino
 */
#pragma once
#ifndef _UICANVAS_H_
#define _UICANVAS_H_

#include <memory>
#include <vector>

class UIObject;
class UIInput;

using UIPtr = std::unique_ptr<UIObject>;

class UICanvasBase {
protected:
	std::vector<UIPtr> uiArray;

public:
	UICanvasBase() = default;
	virtual ~UICanvasBase() = default;

	virtual void Init() = 0;
	virtual void Update(float _t,UIInput& _input);
	virtual void Render();

	/*
	 * @brief UIオブジェクトの生成
	 */
	template<class T,class... Args>
	T* Instantiate(Args&&... _args);

	template<class T, class... Args>
	std::unique_ptr<T> UniqueInstantiate(Args&&... _args);
};

#endif // !_UICANVAS_H_

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
