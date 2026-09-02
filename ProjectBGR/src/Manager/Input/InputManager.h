/*
 * @brief 入力管理クラス
 * @author Sekino
 */
#pragma once
#include "DesignPattern/Singleton/Singleton.h"	
#include "../ManagerBase.h"
#include "../../Library/Vector/Vector3.h"
#include <array>
#include <memory>

class PadBase;

constexpr int KEY_NUM = 256;
constexpr int MAX_PORT_NUM = 4;

class InputManager : public ManagerBase, public Singleton<InputManager> {
private:
	// キーボードの入力状況
	char currentKeyState[KEY_NUM];
	char prevKeyState[KEY_NUM];

	// マウスの入力状況
	int currentMouseState;
	int prevMouseState;
	int nowMousePosX;		// 現在のマウス位置X
	int prevMousePosX;		// 直前のマウス位置X
	int nowMousePosY;		// 現在のマウス位置Y
	int prevMousePosY;		// 直前のマウス位置Y
	bool mouseVisible = true;		// マウスカーソルの表示非表示フラグ
	bool prevInputMouse;	// 直前の入力がマウスかどうか
	bool mouseMoveSkip;		// マウスの中央固定を1フレームスキップするためのフラグ

	// コントローラ管理配列
	std::array<std::unique_ptr<PadBase>, MAX_PORT_NUM> pads;

private:
	// 初期化処理
	void Start() override;
public:
	InputManager();
	~InputManager();

	// 更新処理
	void Update(float _t) override;

private:
	/*
	 * @brief コントローラの更新
	 */
	void UpdatePad();

	/*
	 * @brief マウスの更新
	 */
	void UpdateMousePointer();
public:
#pragma region KeyBoard
	/**
	押されているか
	KEY_INPUT_##
	*/
	inline bool IsKey(int _key) const { return currentKeyState[_key]; }
	/**
	押されたか
	KEY_INPUT_##
	*/
	inline bool IsKeyDown(int _key) const { return currentKeyState[_key] && !prevKeyState[_key]; }
	/**
	離したか
	KEY_INPUT_##
	*/
	inline bool IsKeyUp(int _key) const { return  !currentKeyState[_key] && prevKeyState[_key]; }
#pragma endregion

#pragma region Mouse

	/**
	  押されているか
	  MOUSE_INPUT_##
	*/
	inline bool IsMouse(int _mouse) const { return currentMouseState & _mouse; }
	/**
	  押されたか
	  MOUSE_INPUT_##
	*/
	inline bool IsMouseDown(int _mouse) const { return (currentMouseState & _mouse) && !(prevMouseState & _mouse); }
	/**
	  離したか
	  MOUSE_INPUT_##
	*/
	inline bool IsMouseUp(int _mouse) const { return !(currentMouseState & _mouse) && (prevMouseState & _mouse); }

	/*
	 * @brief マウスの移動量
	 */
	Vector3 GetMouseMove() const;

	/*
	 * @brief マウスの位置取得
	 */
	Vector3 GetMousePos() const;

#pragma endregion

	// コントローラの取得
	PadBase* GetPad(int _index) const { return pads[_index].get(); }

	// XINPUTのボタン番号をDirectInputのボタン番号に変換
	int ExchangeXInputButton(int _XINPUT, int _padNum);
};