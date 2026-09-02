/*
 * @brief デバッグ用フリーカメラ
 * @author Sekino
 */
#pragma once
#ifndef _CAMERAFREEMOVEMENT_H_
#define _CAMERAFREEMOVEMENT_H_

#include "../CameraMovementBase.h"
class CameraFreeMovement : public CameraMovementBase {
	void Move(Transform* _attachObjectTransform, float _t) override;
};
#endif // !_CAMERAFREEMOVEMENT_H_