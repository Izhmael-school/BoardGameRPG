/*
 * @brief テキストの基底
 * @author Sekino
 */
#pragma once
#ifndef _UITEXT_H_
#define _UITEXT_H_

#include "../UIObject.h"
#include "Style/UITextStyle.h"
#include <string>

class UIText : public UIObject {
protected:
	// 描画するテキスト
	std::string text;
	// テキストの見た目
	UITextStyle style;
	// フォントのハンドル
	int fontHandle;
	// 選択しているか
	bool isSelect;
	// 選択できるか
	bool canSelect;
	// 描画位置調節用
	Vector2 basePos;
public:
	UIText();
	UIText(const std::string _text, UITextStyle _style, const Vector2& _pos,const std::string& _name = "");
	virtual ~UIText() override = default;

protected:
	void OnInit() override;
	void OnUpdate(float _t) override;
	void OnRender() override;
	void OnEnd() override;

	/*
	 * @brief スタイルの描画位置に合わせる
	 */
	void Reposition();
public:
	/*
	 * @brief FontManagerからフォントハンドルを取得
	 */
	void Build();
	
	/*
	 * @breif 選択状態を設定
	 */
	inline void SetSelect(bool _isSelect) { isSelect = _isSelect; }

	/*
	 * @brief 選択可否を設定
	 */
	inline void SetCanSelect(bool _canSelect) { canSelect = _canSelect; }

	/*
	 * @brief テキストの変更
	 */
	inline void SetText(const std::string& _text) { text = _text; Reposition(); }

	/*
	 * @brief テキストの取得
	 */
	inline const std::string& GetText() const { return text; }

	/*
	 * @brief フォントハンドルの取得
	 */
	inline int GetFontHandle() const { return fontHandle; }
};

#endif // !_UITEXT_H_