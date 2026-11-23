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
	bool isFit;
	bool isCenter;

public:
	UIWindow(int _backColor, int _frameColor, int _x, int _y, int _w, int _h, bool _isCenter = true, int _frameSize = 10, std::string _uiName = "");
	UIWindow(std::string _text, UITextStyle _textStyle, int _backColor, int _frameColor, int _x, int _y, int _frameSize = 10, std::string _uiName = "");
	UIWindow(std::string _name);
protected:
	void OnInit() override;

	void OnUpdate(float _t, UIInput& _input) override;

	void OnRender() override;

	void OnEnd() override;

public:
	void SetText(const std::string& _text);

	UIText* GetTextUI() const { return textUI.get(); }

	void SetBackColor(int _color) { backColor = _color; }
	void SetFrameColor(int _color) { frameColor = _color; }
	void SetWidth(int _w) { w = _w; }
	void SetHeight(int _h) { h = _h; }
	void SetIsCenter(bool _isCenter) { isCenter = _isCenter; }
	void SetFrameSize(int _frameSize) { frameSize = _frameSize; }
};

#endif // !_UIWINDOW_H_