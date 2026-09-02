/*
 * @brief フリーカメラオブジェクト
 * @author Sekino
 */
#pragma once
#ifndef _FREECAMERA_H_
#define _FREECAMERA_H_

#include "../GameObject.h"

class FreeCamera : public GameObject {
public:
	FreeCamera(int _modelHandle = -1,const std::string& _name = "FreeCamera", Vector3 _pos = VZero, Vector3 _rot = VZero, Tag _tag = Camera);
	~FreeCamera() = default;

private:
	void Start() override;

	void SetClassID() override;
};

#endif