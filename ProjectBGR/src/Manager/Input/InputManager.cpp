/*
 * @file InputManager.cpp
 * @author Sekino
 */
#include "InputManager.h"
#include "Pad/DirectInput/DirectInputPad.h"
#include "Pad/XInput/XInputPad.h"
#include "Pad/PadBase.h"
#include "DxLib.h"
#include <imgui.h>

InputManager::InputManager()
	:currentKeyState{}
	, prevKeyState{}
	, currentMouseState(-1)
	, prevMouseState(-1) 
{
	Start();
}

InputManager::~InputManager()
{
}

void InputManager::Start() {
	memset(currentKeyState, 0, KEY_NUM);
	memset(prevKeyState, 0, KEY_NUM);
}

void InputManager::Update(float _t) {
	// キーの更新
	memcpy_s(prevKeyState, KEY_NUM, currentKeyState, KEY_NUM);
	GetHitKeyStateAll(currentKeyState);

	// マウスの更新
	prevMouseState = currentMouseState;
	currentMouseState = GetMouseInput();

	// コントローラの更新
	UpdatePad();

	// マウスポインターの更新
	UpdateMousePointer();

	ImGui::Begin("MousePointerMove");
	Vector3 move = GetMouseMove();
	ImGui::Text("x:%03f | y:%03f", move.x, move.y);
	ImGui::End();

}

void InputManager::UpdatePad() {
	for (int i = 0; i < MAX_PORT_NUM; i++) {
		// パッドが存在しない場合は作成
		if (!pads[i]) {
			int index = DX_INPUT_PAD1 + i;
			XINPUT_STATE xi;
			// -1じゃなければXInputとして扱う
			int result = GetJoypadXInputState(index, &xi);
			if (result != -1)
				pads[i] = std::make_unique<XInputPad>(index);
			else {
				DINPUT_JOYSTATE di;
				result = GetJoypadDirectInputState(index, &di);
				// -1じゃなければDirectInputとして扱う
				if(result != -1)
					pads[i] = std::make_unique<DirectInputPad>(index);
			}
		}

		// パッドを更新
		if (pads[i]) {
			pads[i]->Update();

			// 接続していない場合はリセット
			if (!pads[i]->IsConnect()) {
				pads[i].reset();
			}
		}
	}
}

void InputManager::UpdateMousePointer() {
	// マウスの表示非表示切り替え
	SetMouseDispFlag(mouseVisible);

	// 1フレーム前のマウスの位置を保存
	prevMousePosX = nowMousePosX;
	prevMousePosY = nowMousePosY;
	// 現在のマウスの位置を保存
	GetMousePoint(&nowMousePosX, &nowMousePosY);
	// マウスカーソルが非表示ならカーソルは中央固定
	if (!mouseVisible) {
		// 1フレーム目はスキップ
		if (mouseMoveSkip) {
			mouseMoveSkip = false;
		}
		else {
			// 画面中央
			int windowWidthCenter = 1920 / 2;
			int windowHeightCenter = 1080 / 2;
			// マウスを画面中央に固定
			SetMousePoint(windowWidthCenter, windowHeightCenter);
			// 1フレーム前位置は画面中央とする
			prevMousePosX = windowWidthCenter;
			prevMousePosY = windowHeightCenter;
		}
	}

	// 直前の入力がマウスかどうか管理
	for (auto key : currentKeyState) {
		if (key == 1) {
			prevInputMouse = false;
			break;
		}
	}

	if (currentMouseState != 0 ||
		prevMousePosX - nowMousePosX != 0 ||
		prevMousePosY - nowMousePosY != 0) {
		prevInputMouse = true;
	}
}

Vector3 InputManager::GetMouseMove() const {
	Vector3 move = VZero;
	move.x = prevMousePosX - nowMousePosX;
	move.y = prevMousePosY - nowMousePosY;
	return move;
}

Vector3 InputManager::GetMousePos() const {
	Vector3 pos = VZero;
	if (!mouseVisible) {
		pos.x = -1;
		pos.y = -1;
	}
	else {
		pos.x = nowMousePosX;
		pos.y = nowMousePosY;
	}
	return pos;
}

int InputManager::ExchangeXInputButton(int _XINPUT, int _padNum) {
	int padType = GetJoypadType(_padNum);

	int buttonNum = -1;

	switch (padType) {
	case DX_PADTYPE_SWITCH_PRO_CTRL:
		switch (_XINPUT) {
		case XINPUT_BUTTON_A:
			break;
		case XINPUT_BUTTON_B:
			break;
		case XINPUT_BUTTON_X:
			break;
		case XINPUT_BUTTON_Y:
			break;
		case XINPUT_BUTTON_START:
			break;
		case XINPUT_BUTTON_BACK:
			break;
		case XINPUT_BUTTON_RIGHT_SHOULDER:
			break;
		case XINPUT_BUTTON_RIGHT_THUMB:
			break;
		case XINPUT_BUTTON_LEFT_SHOULDER:
			break;
		case XINPUT_BUTTON_LEFT_THUMB:
			break;
		default:
			break;
		}
		break;
	case DX_PADTYPE_OTHER:
		break;
	default:
		break;
	}

	return buttonNum;
}
