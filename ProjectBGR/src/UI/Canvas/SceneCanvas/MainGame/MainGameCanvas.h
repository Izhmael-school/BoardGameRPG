/*
 * @brief メインゲームシーン用のキャンバス
 * @author Sekino
 */
#pragma once
#ifndef _MAINGAMECANVAS_H_
#define _MAINGAMECANVAS_H_

#include "../../UICanvasBase.h"

class MainGameCanvas : public UICanvasBase {
private:


public:
	void Init() override;
};

#endif // !_MAINGAMECANVAS_H_