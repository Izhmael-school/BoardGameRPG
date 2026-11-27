/*
 * @brief 基本的な定点カメラ
 * @author Sekino
 */
#pragma once
#ifndef _NORMALCAMERA_H_
#define _NORMALCAMERA_H_

#include "../GameObject.h"
class NormalCamera : public GameObject {
public:
	NormalCamera(int _modelHandle = -1, const std::string& _name = "NormalCamera", Vector3 _pos = VZero, Vector3 _rot = VZero, Tag _tag = Camera);
	~NormalCamera() = default;

private:
	void Start() override;

	void SetClassID() override;
};

#endif // !_NORMALCAMERA_H_