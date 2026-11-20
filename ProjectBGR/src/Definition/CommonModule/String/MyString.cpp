#include "MyString.h"
#include "DxLib.h"
#include <windows.h>
#include <iostream>

void MyString::StringCenterPos(const std::string& _str, int _fontHandle, int* posX, int* posY, float exRateX, float exRateY) {
	int w, h, line;
	const TCHAR* str = _str.c_str();

	if (_fontHandle != -1)
		GetDrawStringSizeToHandle(&w, &h, &line, str, (int)_tcslen(str), _fontHandle);
	else
		GetDrawStringSize(&w, &h, &line, str, (int)_tcslen(str));

	*posX -= (int)(w * exRateX) / 2;
	*posY -= (int)(h * exRateY) / 2;
}

int MyString::StringRightPos(const std::string& _str, int _fontHandle, int posX, float exRateX) {
	int w, h, line;
	const TCHAR* str = _str.c_str();

	if (_fontHandle != -1)
		GetDrawStringSizeToHandle(&w, &h, &line, str, (int)_tcslen(str), _fontHandle);
	else
		GetDrawStringSize(&w, &h, &line, str, (int)_tcslen(str));

	return posX -= (int)(w * exRateX);
}

int MyString::GetUTF8CharLenght(unsigned char c) {
	if ((c & 0x80) == 0x00) {
		return 1;
	}
	if ((c & 0xE0) == 0xC0) {
		return 2;
	}
	if ((c & 0xF0) == 0xE0) {
		return 3;
	}
	if ((c & 0xF8) == 0xF0) {
		return 4;
	}

	return 1;
}

std::string MyString::GetUTF8Char(const std::string& str, int& index) {
	if(str.empty() || index < 0 || index >= static_cast<int>(str.size()))
		return "";

	int length = GetUTF8CharLenght(static_cast<unsigned char>(str[index]));

	std::string result = str.substr(index, length);

	index += length;

	return result;
}

std::string MyString::Zen2Han(const std::string& _str) {
    // UTF-8 → UTF-16
    int size = MultiByteToWideChar(
        CP_UTF8, 0,
        _str.c_str(), -1,
        nullptr, 0
    );

    if (size == 0) return _str;

    std::wstring src(size, L'\0');

    MultiByteToWideChar(
        CP_UTF8, 0,
        _str.c_str(), -1,
        src.data(), size
    );

    // 変換後の文字列サイズを取得
    int resultSize = LCMapStringW(
        LOCALE_USER_DEFAULT,
        LCMAP_HALFWIDTH,
        src.c_str(), -1,
        nullptr, 0
    );

    if (resultSize == 0) return _str;

    std::wstring result(resultSize, L'\0');

    // 全角 → 半角
    LCMapStringW(
        LOCALE_USER_DEFAULT,
        LCMAP_HALFWIDTH,
        src.c_str(), -1,
        result.data(), resultSize
    );

    // UTF-16 → UTF-8
    int utf8Size = WideCharToMultiByte(
        CP_UTF8, 0,
        result.c_str(), -1,
        nullptr, 0, nullptr, nullptr
    );

    if (utf8Size == 0) return _str;

    std::string output(utf8Size, '\0');

    WideCharToMultiByte(
        CP_UTF8, 0,
        result.c_str(), -1,
        output.data(), utf8Size,
        nullptr, nullptr
    );

    output.pop_back(); // 終端文字を除去

    return output;
}
