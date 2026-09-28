#include "FontManager.h"
#include "DxLib.h"

FontManager::FontManager() {
}

FontManager::~FontManager() {
}


int FontManager::CreateFontData(const std::string& _fontName, int _size, int _thick, int _fontType, int _edgeSize, int _italic) {
	FontData data;

	// フォント生成
	int f = CreateFontToHandle(_fontName.c_str(), _size, _thick, _fontType,DX_CHARSET_UTF8,_edgeSize,_italic);

	// フォントが無ければ帰る
	if (f == -1) return -1;

	data.fontHandle = f;

	data.fontName = _fontName;

	data.fontID = GetFontID(_size, _thick, _fontType, _edgeSize, _italic);

	// 配列に登録
	fontDataArray.push_back(data);

	return fontDataArray.back().fontHandle;
}

int FontManager::GetFontHandle(const std::string& _fontName, int _size, int _thick, int _fontType, int _edgeSize, int _italic) {
	uint32_t id = GetFontID(_size, _thick, _fontType, _edgeSize, _italic);

	for (auto& f : fontDataArray) {
		if (f.fontName != _fontName) continue;
		if (f.fontID != id) continue;
		
		return f.fontHandle;
	}

	return CreateFontData(_fontName,_size,_thick,_fontType,_edgeSize,_italic);
}

void FontManager::DeleteFont() {

	for (auto f : fontDataArray) {
		DeleteFontToHandle(f.fontHandle);
	}

	fontDataArray.clear();
	fontDataArray.shrink_to_fit();
}

uint32_t FontManager::GetFontID(int _size, int _thick, int _fontType, int _edgeSize, int _italic) {
	uint32_t id = 0;

	id |= (uint32_t)(_size & 0x3ff);	// 10ビット分
	id |= (uint32_t)(_edgeSize & 0x3ff) >> 10;	// 10ビット分
	id |= (uint32_t)(_fontType & 0x3f) >> 20;	// 6ビット分
	id |= (uint32_t)(_fontType & 0xf) >> 26;	// 4ビット分
	id |= (uint32_t)(_fontType & 0x1) >> 30;	// 1ビット分

	return id;
}
