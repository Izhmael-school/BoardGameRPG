/*
 * @brief UIのウィンドウ
 * @author Sekino
 */
#pragma once
#ifndef _UIWINDOW_H_
#define _UIWINDOW_H_

#include "../UIObject.h"
#include "../UIText/UIText.h"
#include <string>
#include <memory>

class UIWindow : public UIObject {
private:
	int dx, dy, w, h;
	std::string text;
	UITextStyle textStyle;
	std::unique_ptr<UIText> textUI;
	int backColor;
	int frameColor;
	int frameSize;

public:
	UIWindow(int _backColor, int _frameColor, int x, int y, int w, int h, std::string _uiName = "");
	UIWindow(std::string _text, UITextStyle _textStyle, int _backColor, int _frameColor, int x, int y, int frameSize = 10, std::string _uiName = "");

protected:
	void OnInit() override;

	void OnUpdate(float _t, UIInput& _input) override;

	void OnRender() override;

	void OnEnd() override;

public:
	void SetText(const std::string& _text);

	UIText* GetTextUI() const { return textUI.get(); }
};

#endif // !_UIWINDOW_H_