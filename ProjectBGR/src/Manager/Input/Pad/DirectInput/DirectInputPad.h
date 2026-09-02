/*
 * @brief DirectInputのコントローラ
 * @author Sekino
 */
#pragma once
#ifndef _DIRECTINPUTPAD_H_
#define _DIRECTINPUTPAD_H_

#include "../PadBase.h"
#include "DxLib.h"

constexpr int DI_PUSH_NUM = 128;

class DirectInputPad : public PadBase {
private:
	// コントローラ(XInput)の入力状況
	DINPUT_JOYSTATE currentPadState{};
	DINPUT_JOYSTATE prevPadState{};

	int padType;	// コントローラーの種類(SwitchとかXBoxとか)

private:
	void Start();

	float StickNorm(int _v);

public:
	DirectInputPad(int _portNum);
	~DirectInputPad() = default;

	void Update() override;

#pragma region Pad

	/**
	  押されているか
	  XINPUT_BUTTON_##
   */
	bool IsPad(int _pad) const override;
	/**
	  押されたか
	  MOUSE_BUTTON_##
	*/
	bool IsPadDown(int _pad) const override;
	/**
	  離したか
	  MOUSE_BUTTON_##
	*/
	inline bool IsPadUp(int _pad) const override;
#pragma endregion
};
#endif