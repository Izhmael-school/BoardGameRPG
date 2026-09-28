#include "MyString.h"
#include "DxLib.h"

void MyString::StringCenterPos(const std::string& _str, int _fontHandle, int* posX, int* posY, int exRateX, int exRateY) {
	int w, h, line;
	const TCHAR* str = _str.c_str();

	if (_fontHandle != -1)
		GetDrawStringSizeToHandle(&w, &h, &line, str, (int)_tcslen(str), _fontHandle);
	else
		GetDrawStringSize(&w, &h, &line, str, (int)_tcslen(str));

	*posX -= (int)(w * exRateX) / 2;
	*posY -= (int)(h * exRateY) / 2;
}

int MyString::StringRightPos(const std::string& _str, int _fontHandle, int posX, int exRateX) {
	int w, h, line;
	const TCHAR* str = _str.c_str();

	if (_fontHandle != -1)
		GetDrawStringSizeToHandle(&w, &h, &line, str, (int)_tcslen(str), _fontHandle);
	else
		GetDrawStringSize(&w, &h, &line, str, (int)_tcslen(str));

	return posX -= (int)(w * exRateX);
}
