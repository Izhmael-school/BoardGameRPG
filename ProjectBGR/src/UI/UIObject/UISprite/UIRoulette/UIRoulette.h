/*
 * @brief ルーレットのUI
 * @author Sekino
 */
#pragma once
#ifndef _UIROULETTE_H_
#define _UIROULETTE_H_

#include "../UISprite.h"

class UIRoulette : public UISprite {
private:
	// ルーレットの針の画像ハンドル
	int arrowHandle;
	// 針の座標
	Vector2 arrowPos;
	// 回転位置
	int acx, acy;
	// 回転
	float aAngle;
	// 拡縮
	int aex, aey;

public:
	UIRoulette(int _rouletteHandle, int _arrowHandle, Vector2 _pos = VZero_2, float _ex = 1.0f, float _ey = 1.0f, std::string _name = "");
	~UIRoulette() override = default;

protected:
	void OnRender() override;

public:
	void SetArrowAngle(float _angle);
};

#endif // !_UIROULETTE_H_