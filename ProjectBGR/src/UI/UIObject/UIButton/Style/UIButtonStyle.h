#pragma once
#include "../../UIText/Style/UITextStyle.h"

class UIButtonStyle {
public:
	// 文字のスタイル
	UITextStyle textStyle;
	// 通常時の画像ハンドル
	int graphHandle;
	// 選択時の画像ハンドル
	int selectGraphHandle;
	// 選択できないときの画像ハンドル
	int cantSelectGraphHandle;
	// 画像の拡縮
	int ex, ey;
	// マウスでクリックできる範囲(中心から)
	int cx, cy;
};