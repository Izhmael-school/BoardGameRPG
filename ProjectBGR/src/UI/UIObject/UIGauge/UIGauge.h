/*
 * @brief UIのゲージ
 * @author Sekino
 */
#pragma once
#ifndef _UIGAUGE_H_
#define _UIGAUGE_H_

#include "../UIObject.h"
#include <algorithm>

enum DecreasingDirection {
	DD_Left,
	DD_Right,
	DD_Top,
	DD_Bottom
};

class UIGauge : public UIObject {
private:
	int w, h;
	float value;
	DecreasingDirection dd;
	int cx, cy;
	int l, r, t, b;

public:
	UIGauge(Vector2 _pos, int _w, int _h ,float _value = 1.0f,DecreasingDirection _dd = DD_Right,std::string _name = "");

	void OnUpdate(float _t, UIInput& _input);

	void OnRender();

public:
	void SetValue(float _value) { value = std::clamp(_value,0.0f,1.0f); }

private:
	void CalcCenter();

	int CalcDecreasingPos();
};

#endif // !_UIGAUGE_H_