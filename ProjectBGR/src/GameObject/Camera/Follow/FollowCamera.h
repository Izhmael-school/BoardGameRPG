/*
 * @brief 対象を追従するカメラ
 * @author Sekino
 */
#pragma once
#ifndef _FOLLOWCAMERA_H_
#define _FOLLOWCAMERA_H_

#include "../NormalCamera.h"
class FollowCamera : public NormalCamera {
private:

public:
	FollowCamera(int _modelHandle = -1, const std::string& _name = "FreeCamera", Vector3 _pos = VZero, Vector3 _rot = VZero, Tag _tag = Camera);
	~FollowCamera() = default;

private:
	void Start() override;

	void SetClassID() override;

public:
	void Setup() override;

	void SetFollowTarget(GameObject* _target);

	void SetDistance(Vector3 _distance);
};

#endif // !_FOLLOWCAMERA_H_