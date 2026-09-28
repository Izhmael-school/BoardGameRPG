/*
 * @brief ボタンの規定
 * @author Sekino
 */
#pragma once
#ifndef _UIBUTTON_H_
#define _UIBUTTON_H_

#include "../UIObject.h"
#include "Style/UIButtonStyle.h"
#include <string>
#include <functional>

class UIInput;

class UIButton : public UIObject {
protected:
	// 選択されているか
	bool isSelect;
	// 選択できるか
	bool canSelect;
	// 決定時のイベント
	std::function<void()> onClick;
	// スタイル
	UIButtonStyle style;
	// 通常テキスト
	std::string text;
	// 選択時テキスト
	std::string selectText;
	// 選択できないときのテキスト
	std::string cantSelectText;
	// フォントハンドル
	int fontHandle;
	// テキスト中心
	int cx, cy;

public:
	UIButton(Vector2 _pos, UIButtonStyle _style, const std::string& _text = "text",const std::string& _selectText = "" , const std::string& _cantSelectText = "");
	virtual ~UIButton() override = default;

protected:
	virtual void OnInit() override;
	virtual void OnUpdate(float _t,UIInput& _input) override;
	virtual void OnRender() override;
	virtual void OnEnd() override;

	/*
	 * @brief マウスカーソルが乗ってるか
	 */
	bool OnMouse(Vector2 _mousePos);

	/*
	 * @brief クリックされた
	 */
	void OnClick();

	/*
	 * @brief テキストの描画位置更新
	 */
	void RePositionText(const std::string& _drawText);
public:

	void SetSelect(bool _isSelect) { isSelect = _isSelect; };

	void SetCanSelect(bool _canSelect) { canSelect = _canSelect; }

	void SetClickEvent(std::function<void()> _func) { onClick = _func; }

};

#endif // !_UIBUTTON_H_