/*
 * @brief メインゲームシーン用のキャンバス
 * @author Sekino
 */
#pragma once
#ifndef _MAINGAMECANVAS_H_
#define _MAINGAMECANVAS_H_

#include "../../UICanvasBase.h"

class UIGauge;

class MainGameCanvas : public UICanvasBase {
private:
	UIGauge* gauge;

public:
	void Init() override;

	void Update(float _t, UIInput& _input) override;
};

#endif // !_MAINGAMECANVAS_H_