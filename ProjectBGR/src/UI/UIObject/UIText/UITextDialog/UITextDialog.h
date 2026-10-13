/*
 * @brief テキストダイアログUI
 * @author Sekino
 */
#pragma once
#ifndef _UITEXTDIALOG_H_
#define _UITEXTDIALOG_H_

#include <vector>
#include <string>
#include "../Style/UITextStyle.h"
#include "../UItext.h"

class UITextDialog : public UIText{
private:
	// 表示する文字列の待機場
	std::vector<std::string> waitStrings;
	// 次の文字が出るまでの時間
	float nextCharTime;
	// 経過時間
	float currentTime;
	// 指定の番号
	int currentCharIndex;
	// 描画しているテキスト
	std::string drawString;
	// 待機列の最前列の文字列をすべて描画したか
	bool isAllDraw;

public:
	UITextDialog(UITextStyle _style, const Vector2& _pos,int backImage = -1);

protected:
	void OnUpdate(float _t, UIInput& _input) override;

	void OnRender() override;

public:
	void AddText(const std::string& _text) { waitStrings.emplace_back(_text); }
}; 

#endif // !_UITEXTDIALOG_H_