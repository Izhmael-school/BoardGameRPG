#include "FreeCamera.h"
#include "Component/Camera/Camera.h"

FreeCamera::FreeCamera(int _modelHandle, const std::string& _name, Vector3 _pos, Vector3 _rot, Tag _tag)
	:GameObject(_modelHandle, _name, _pos, _rot, VOne, _tag) {
	Start();
}

void FreeCamera::Start() {
	class Camera* camera = AddComponent<class Camera>(CameraMovementMode::Free);
	camera->SetNearFar(10.0f);
	SetClassID();
	size_t a = classID.hash_code();
}

void FreeCamera::SetClassID() {
	classID = typeid(this);
}
