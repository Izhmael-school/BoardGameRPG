/*
 * @brief カメラの動きの基底
 * @author Sekino
 */
#pragma once
#ifndef _CAMERAMOVEMENTBASE_H_
#define _CAMERAMOVEMENTBASE_H_

class Transform;

class CameraMovementBase {
public:
	virtual void Move(Transform* _attachObjectTransform,float _t) = 0;
};

#endif