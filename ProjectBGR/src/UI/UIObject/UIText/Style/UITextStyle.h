/*
 * @brief テキストUIの見た目を決める
 * @author Sekino
 */
#pragma once
#ifndef _UITEXTSTYLE_H_
#define _UITEXTSTYLE_H_

#include <string>

enum TextDrawPosition {
	TextDrawPosition_Left,
	TextDrawPosition_Center,
	TextDrawPosition_Right
};

enum FontType {
	FontType_Normal = 0,
	FontType_Edge = 1,
	FontType_Antialiasing = 2,
	FontType_Antialiasing_4x4 = 18,
	FontType_Antialiasing_8x8 = 34,
	FontType_Antialiasing_16x16 = 50,
	FontType_Antialiasing_Edge = 3,
	FontType_Antialiasing_Edge_4x4 = 19,
	FontType_Antialiasing_Edge_8x8 = 35,
	FontType_Antialiasing_Edge_16x16 = 51,
};

class UITextStyle {
public:
	// 通常時の色
	unsigned int normalColor = 0xffffff;
	// 縁の色
	unsigned int outlineColor = 0x000000;
	// 選択時の色
	unsigned int selectedColor = 0xffff00;
	// 選択できないときの色
	unsigned int cantSelectColor = 0x707070;
	// フォント名
	std::string fontName = "Meiryo";
	// フォントサイズ
	int fontSize = 18;
	// フォントの太さ
	int fontThickness = 5;
	// エッジのサイズ
	int edgeSize = 0;
	// イタリック
	int italic = 0;
	// 描画位置
	TextDrawPosition textDrawPosition = TextDrawPosition_Left;
	// フォントのタイプ
	FontType fontType = FontType_Normal;

public:
	UITextStyle(unsigned int _nColor,unsigned int _oColor,unsigned int _sColor,unsigned int _cColor,std::string _fontName,int _fontSize,int _fontThick,int _edgeSize,int _italic,TextDrawPosition _textDrawPosition,FontType _fontType);
	UITextStyle() = default;
};

#endif // !_UITEXTSTYLE_H_