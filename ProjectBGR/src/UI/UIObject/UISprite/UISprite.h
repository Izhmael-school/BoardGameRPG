/*
 * @brief 画像の基底
 * @author Sekino
 */
#pragma once
#ifndef _UISPRITE_H_
#define _UISPRITE_H_

#include "../UIObject.h"
class UISprite : public UIObject {
private:
	int graphHandle;
	// 回転位置
	int cx, cy;
	// 回転
	int angle;
	// 拡縮
	int ex, ey;

public:
	UISprite(int _handle, Vector2 _pos,float _ex = 1.0f,float _ey = 1.0f, float _rot = 0);
	virtual ~UISprite() override = default;

protected:
	virtual void OnInit() override;
	virtual void OnUpdate(float _t) override;
	virtual void OnRender() override;
	virtual void OnEnd() override;

public:
	void SetGraphHandle(int _handle) { graphHandle = _handle; }
	int GetGraphHandle() const {return graphHandle;}
};

#endif // !_UISPRITE_H_