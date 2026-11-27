/*
 * @brief 指定対象を追従する移動
 * @author Sekino
 */

#pragma once
#ifndef _CAMERAFOLLOWMOVEMENT_H_
#define _CAMERAFOLLOWMOVEMENT_H_

#include "../CameraMovementBase.h"
#include "Vector3.h"

class GameObject;

class CameraFollowMovement : public CameraMovementBase {
private:
	GameObject* target;
	Vector3 before;
	Vector3 after;
	Vector3 distance;
	float time;

public:
	void Move(Transform* _attachObjectTransform, float _t) override;

	void SetFollowingTarget(GameObject* _target) { target = _target; }

	void SetDistance(Vector3 _distance) { distance = _distance; }

	void Reset() override;
};

#endif