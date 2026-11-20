/*
 * @brief XInputのコントローラ
 * @author Sekino
 */
#pragma once
#include "../PadBase.h"
#include "DxLib.h"

class XInputPad : public PadBase {
private:
	// コントローラ(XInput)の入力状況
	XINPUT_STATE currentPadState{};
	XINPUT_STATE prevPadState{};

public:
	XInputPad(int _portNum);
	~XInputPad() = default;

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

	inline bool IsCurrentState(int _pad)const { return currentPadState.Buttons[_pad] == 0; }

	inline bool IsPrevState(int _pad)const { return prevPadState.Buttons[_pad] == 0; }
#pragma endregion
};

