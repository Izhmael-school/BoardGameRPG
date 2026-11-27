#include "CameraFollowMovement.h"
#include "../../../Transform/Transform.h"
#include <algorithm>
#include "GameObject/GameObject.h"

void CameraFollowMovement::Move(Transform* _attachObjectTransform, float _t) {
	// 対象がいなければ帰る
	if (!target) return;

	Vector3 now = _attachObjectTransform->GetPosition();
	if (!before.Equal(now)) {
		before = now;
		time = 0;
	}

	after = target->GetTransform()->GetPosition();

	Vector3 newPos = Vector3::Lerp(before, Vector3::VAdd(after,distance), time);

	_attachObjectTransform->SetPosition(newPos);

	time = std::clamp(time + (_t * 5),0.0f,1.0f);
}

void CameraFollowMovement::Reset() {
	target = nullptr;
	before = VZero;
	after = VZero;
	time = 0.0f;
	distance = VZero;
}
