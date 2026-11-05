#include "MyString.h"
#include "DxLib.h"

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
