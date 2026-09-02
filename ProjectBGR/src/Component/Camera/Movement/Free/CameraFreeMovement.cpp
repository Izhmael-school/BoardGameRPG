#include "CameraFreeMovement.h"
#include "Component/Transform/Transform.h"
#include "Manager/Input/InputManager.h"
#include "Definition/Const/KeyInputConst.h"

void CameraFreeMovement::Move(Transform* _attachObjectTransform, float _t) {
	InputManager& input = InputManager::GetInstance();
	Transform* transform = _attachObjectTransform;

	// 右クリックしてる時に移動
	if (input.IsMouse(2)) {
		// シフト押してたら加速
		int dash = input.IsKey(KEY_INPUT_LSHIFT) ? 2 : 1;
		// 移動
		if (input.IsKey(KEY_INPUT_W))
			transform->AddPosition(transform->GetForward(), 10.0f * dash);
		if (input.IsKey(KEY_INPUT_A))
			transform->AddPosition(transform->GetRight(), -10.0f * dash);
		if (input.IsKey(KEY_INPUT_S))
			transform->AddPosition(transform->GetForward(), -10.0f * dash);
		if (input.IsKey(KEY_INPUT_D))
			transform->AddPosition(transform->GetRight(), 10.0f * dash);
		if (input.IsKey(KEY_INPUT_E))
			transform->AddPosition(transform->GetUp(), 10.0f * dash);
		if (input.IsKey(KEY_INPUT_Q))
			transform->AddPosition(transform->GetUp(), -10.0f * dash);
		// マウスの移動量で回転させる
		Vector3 mouseMove = input.GetMouseMove();

		// 回転
		transform->AddRotation(VUp, -mouseMove.x / 5);
		transform->AddRotation(VRight, -mouseMove.y / 5);
	}

}