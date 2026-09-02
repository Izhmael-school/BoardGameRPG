/*
 * @brief コントローラーの基底
 * @author Sekino
 */
#pragma once
#ifndef _PADBASE_H_
#define _PADBASE_H_

#include "Library/Vector/Vector3.h"

constexpr float DEADZONE = 0.15f;
class PadBase {
protected:
	// ポート番号
	int connectIndex;
	// 接続されてるか
	bool isConnect;

	// スティックの位置
	float lx, ly, rx, ry;
	// トリガー
	float lt, rt;

protected:
	// スティックの正規化
	virtual float StickNorm(float _v);

public:
	PadBase(int _connectIndex);
	// 更新処理
	virtual void Update() = 0;
	// 接続されているか
	bool IsConnect() const { return isConnect; }

#pragma region Stick
	Vector3 GetRStick() const { return Vector3(rx, ry, 0); }
	Vector3 GetLStick() const { return Vector3(lx, ly, 0); }
#pragma endregion

#pragma region Trigger
	float GetRTrigger() const { return rt; }
	float GetLTrigger() const { return lt; }
#pragma endregion

#pragma region Pad

	/**
	  押されているか
	  XINPUT_BUTTON_##
   */
	virtual bool IsPad(int _pad) const = 0;
	/**
	  押されたか
	  MOUSE_BUTTON_##
	*/
	virtual bool IsPadDown(int _pad) const = 0;
	/**
	  離したか
	  MOUSE_BUTTON_##
	*/
	virtual bool IsPadUp(int _pad) const = 0;
#pragma endregion
};
#endif // !_PADBASE_H_