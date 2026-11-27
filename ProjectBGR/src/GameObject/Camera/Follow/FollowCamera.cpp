#include "FollowCamera.h"
#include "Component/Camera/Camera.h"
#include "Component/Camera/Movement/Linear/CameraFollowMovement.h"

FollowCamera::FollowCamera(int _modelHandle, const std::string& _name, Vector3 _pos, Vector3 _rot, Tag _tag)
	:NormalCamera(_modelHandle, _name, _pos, _rot, _tag) {
	Start();
}

void FollowCamera::Start() {
	class Camera* camera = AddComponent<class Camera>(CameraMovementMode::Follow);
	camera->SetNearFar(10.0f);
	SetClassID();
	size_t a = classID.hash_code();
}

void FollowCamera::SetClassID() {
	classID = typeid(this);
}

void FollowCamera::Setup() {
	auto camera = GetComponent<class Camera>();

	if (camera)
		camera->GetCameraMovement()->Reset();
}

void FollowCamera::SetFollowTarget(GameObject* _target) {
	if (!_target) return;

	auto camera = GetComponent<class Camera>();

	if (!camera) return;
	auto follow = static_cast<CameraFollowMovement*>(camera->GetCameraMovement());
	follow->SetFollowingTarget(_target);
}

void FollowCamera::SetDistance(Vector3 _distance) {
	auto camera = GetComponent<class Camera>();

	if (!camera) return;
	auto follow = static_cast<CameraFollowMovement*>(camera->GetCameraMovement());
	follow->SetDistance(_distance);
}
