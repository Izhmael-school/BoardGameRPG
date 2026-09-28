/*
 * @brief フォントデータの管理と生成
 * @author Sekino
 */
#pragma once
#ifndef _FONTMANAGER_H_
#define _FONTMANAGER_H_

#include "DesignPattern/Singleton/Singleton.h"
#include <vector>
#include <string>

struct FontData {
	int fontHandle;
	std::string fontName;
	uint32_t fontID;
};

class FontManager : public Singleton<FontManager> {
public:
	FontManager();
	~FontManager();

	/// <summary>
	/// フォントを作る
	/// </summary>
	/// <param name="fontName">フォントの名前</param>
	/// <param name="size">サイズ</param>
	/// <param name="thick">太さ</param>
	/// <param name="fontType">フォントタイプ</param>
	int CreateFontData(const std::string& _fontName,int _size,int _thick,int _fontType,int _edgeSize,int _italic = 0);
	int GetFontHandle(const std::string& _fontName, int _size, int _thick, int _fontType, int _edgeSize, int _italic = 0);


	void DeleteFont();

private:
	uint32_t GetFontID(int _size, int _thick, int _fontType, int _edgeSize, int _italic);

	std::vector<FontData> fontDataArray;
};

#endif // !_FONTMANAGER_H_