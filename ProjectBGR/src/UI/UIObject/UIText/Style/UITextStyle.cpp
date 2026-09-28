#include "UITextStyle.h"

UITextStyle::UITextStyle(unsigned int _nColor, unsigned int _oColor, unsigned int _sColor, unsigned int _cColor, std::string _fontName, int _fontSize, int _fontThick, int _edgeSize, int _italic, TextDrawPosition _textDrawPosition, FontType _fontType) 
	:normalColor(_nColor)
	,outlineColor(_oColor)
	,selectedColor(_sColor)
	,cantSelectColor(_cColor)
	,fontName(_fontName)
	,fontSize(_fontSize)
	,fontThickness(_fontThick)
	,edgeSize(_edgeSize)
	,italic(_italic)
	,textDrawPosition(_textDrawPosition)
	,fontType(_fontType)
{

}
