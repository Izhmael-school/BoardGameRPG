#include "CameraFreeMovement.h"
#include "Component/Transform/Transform.h"
#include "Manager/Input/InputManager.h"
#include "Vector2.h"
#include "Vector3.h"

void CameraFreeMovement::Move(Transform* _attachObjectTransform, float _t) {
	InputManager& input = InputManager::GetInstance();
	Transform* transform = _attachObjectTransform;

	// 蜿ｳ繧ｯ繝ｪ繝・け縺励※繧区凾縺ｫ遘ｻ蜍・
	if (input.IsMouse(2)) {
		// 繧ｷ繝輔ヨ謚ｼ縺励※縺溘ｉ蜉騾・
		int dash = input.IsKey(KEY_LSHIFT) ? 2 : 1;
		// 遘ｻ蜍・
		if (input.IsKey(KEY_W))
			transform->AddPosition(transform->GetForward(), 10.0f * dash);
		if (input.IsKey(KEY_A))
			transform->AddPosition(transform->GetRight(), -10.0f * dash);
		if (input.IsKey(KEY_S))
			transform->AddPosition(transform->GetForward(), -10.0f * dash);
		if (input.IsKey(KEY_D))
			transform->AddPosition(transform->GetRight(), 10.0f * dash);
		if (input.IsKey(KEY_E))
			transform->AddPosition(transform->GetUp(), 10.0f * dash);
		if (input.IsKey(KEY_Q))
			transform->AddPosition(transform->GetUp(), -10.0f * dash);
		// 繝槭え繧ｹ縺ｮ遘ｻ蜍暮㍼縺ｧ蝗櫁ｻ｢縺輔○繧・
		Vector2 mouseMove = input.GetMouseMove();

		// 蝗櫁ｻ｢
		transform->AddRotation(VUp, -mouseMove.x / 5);
		transform->AddRotation(VRight, -mouseMove.y / 5);
	}

}