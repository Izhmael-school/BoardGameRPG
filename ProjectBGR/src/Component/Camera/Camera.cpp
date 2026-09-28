#include "Camera.h"
#include "Library/Vector/Vector3.h"
#include "Library/Vector/ConversionVECTOR.h"
#include "Definition/CommonModule/Math/MyMath.h"
#include "Movement/CameraMovementBase.h"
#include "Movement/Free/CameraFreeMovement.h"
#include "GameObject/GameObject.h"
#include "DxLib.h"

#undef near;
#undef far;

Camera::Camera(GameObject* _attachObject, CameraMovementMode _mode, Projection _projection, float _fov, float _near, float _far)
	:ComponentBase(_attachObject)
	, projection(_projection)
	, fov(_fov)
	, near(_near)
	, far(_far) {
	// 謠冗判遽・峇縺ｮ謖・ｮ・
	SetCameraNearFar(near, far);
	// 謠冗判譁ｹ豕輔・謖・ｮ・
	SetProjection(projection, fov);
	// 繧ｫ繝｡繝ｩ縺ｮ遘ｻ蜍輔ｒ逕滓・
	SetMovement(_mode);
}


Camera::~Camera() {
	movement.reset();
}

void Camera::Update(float _t) {
	if (!isActive) return;

	Transform* transform = attachObject->GetTransform();
	movement->Move(transform, _t);

	VECTOR pos = ConversionVECTOR::Vector3ToVECTOR(transform->GetPosition());
	VECTOR rot = ConversionVECTOR::Vector3ToVECTOR(transform->GetRotation());
	SetCameraPositionAndAngle(pos, MyMath::Deg2Rad(rot.x), MyMath::Deg2Rad(rot.y), MyMath::Deg2Rad(rot.z));
	Set3DSoundListenerPosAndFrontPos_UpVecY(pos, VAdd(pos, VGet(0, 0, 1)));
}

void Camera::Render() {
#if _DEBUG
	DrawGizmo(0x0000ff);
#endif
}

void Camera::SetMovement(CameraMovementMode _mode) {
	switch (_mode) {
	case Free:
		movement = std::make_unique<CameraFreeMovement>();
		break;
	default:
		break;
	}
}

void Camera::SetProjection(Projection _projection, float _fov) {
	projection = _projection;
	fov = _fov;

	switch (projection) {
	case Perspective:
		SetupCamera_Perspective(MyMath::Deg2Rad(fov));
		break;
	case Orthographic:
		SetupCamera_Ortho(fov);
		break;
	}
}

void Camera::SetNearFar(float _near, float _far) {
	near = _near;
	far = _far;

	SetCameraNearFar(near, far);
}

void Camera::DrawGizmo(int _color) {
	// 逕ｻ髱｢繧｢繧ｹ繝壹け繝域ｯ斐ｒ蜿門ｾ・
	int sx = 0, sy = 0;
	GetScreenState(&sx, &sy, nullptr);
	float aspect = (sy != 0) ? static_cast<float>(sx) / sy : 1.0f;

	// 迴ｾ蝨ｨ繧ｻ繝・ヨ縺輔ｌ縺ｦ縺・ｋ繧ｫ繝｡繝ｩ縺ｮ菴咲ｽｮ繝ｻ蟋ｿ蜍｢繧貞叙蠕・
	VECTOR camPos = GetCameraPosition();
	VECTOR front = GetCameraFrontVector();
	VECTOR up = GetCameraUpVector();
	VECTOR right = GetCameraRightVector();

	// 荳贋ｸ九・蟾ｦ蜿ｳ縺ｮ蜊雁ｹ・ｒ豎ゅａ繧九Λ繝繝
	auto drawBox = [&](float halfV_n, float halfH_n, float halfV_f, float halfH_f) {
		VECTOR nearCenter = VAdd(camPos, VScale(front, near));
		VECTOR farCenter = VAdd(camPos, VScale(front, far));

		VECTOR ntl = VAdd(VAdd(nearCenter, VScale(up, halfV_n)), VScale(right, -halfH_n));
		VECTOR ntr = VAdd(VAdd(nearCenter, VScale(up, halfV_n)), VScale(right, halfH_n));
		VECTOR nbl = VAdd(VAdd(nearCenter, VScale(up, -halfV_n)), VScale(right, -halfH_n));
		VECTOR nbr = VAdd(VAdd(nearCenter, VScale(up, -halfV_n)), VScale(right, halfH_n));

		VECTOR ftl = VAdd(VAdd(farCenter, VScale(up, halfV_f)), VScale(right, -halfH_f));
		VECTOR ftr = VAdd(VAdd(farCenter, VScale(up, halfV_f)), VScale(right, halfH_f));
		VECTOR fbl = VAdd(VAdd(farCenter, VScale(up, -halfV_f)), VScale(right, -halfH_f));
		VECTOR fbr = VAdd(VAdd(farCenter, VScale(up, -halfV_f)), VScale(right, halfH_f));

		// near髱｢
		DrawLine3D(ntl, ntr, _color);
		DrawLine3D(ntr, nbr, _color);
		DrawLine3D(nbr, nbl, _color);
		DrawLine3D(nbl, ntl, _color);
		// far髱｢
		DrawLine3D(ftl, ftr, _color);
		DrawLine3D(ftr, fbr, _color);
		DrawLine3D(fbr, fbl, _color);
		DrawLine3D(fbl, ftl, _color);
		// near-far謗･邯夊ｾｺ
		DrawLine3D(ntl, ftl, _color);
		DrawLine3D(ntr, ftr, _color);
		DrawLine3D(nbl, fbl, _color);
		DrawLine3D(nbr, fbr, _color);
		};

	if (projection == Perspective) {
		float halfV_near = tanf(fov * 0.5f) * near;
		float halfV_far = tanf(fov * 0.5f) * far;
		drawBox(halfV_near, halfV_near * aspect, halfV_far, halfV_far * aspect);
	}
	else { // Orthographic
		float halfV = fov * 0.5f;
		drawBox(halfV, halfV * aspect, halfV, halfV * aspect);
	}
}
