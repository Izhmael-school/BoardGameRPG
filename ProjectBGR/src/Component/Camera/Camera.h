/*
 * @brief 繧ｫ繝｡繝ｩ繧ｳ繝ｳ繝昴・繝阪Φ繝・
 * @author Sekino
 */
#pragma once
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../ComponentBase.h"
#include "Vector3.h"
#include <memory>

class CameraMovementBase;

enum Projection {
	Perspective,	// 立体
	Orthographic	// 平面
};

// カメラの動かし方
enum CameraMovementMode {
<<<<<<< Updated upstream
	FixedPoint,
	Free,
	Follow
=======
	FixedPoint, // 定点
	Free,		// デバッグ用操作
	Follow		// 追従
>>>>>>> Stashed changes
};

class Camera : public ComponentBase {
private:
	Projection projection;	// 描画方法
	float fov;				// 視野角
	float near;				// 手前グリッチの距離
	float far;				// 奥グリッチの距離

	std::unique_ptr<CameraMovementBase> movement;	// カメラの動き方

public:
	Camera(GameObject* _attachObject, CameraMovementMode _mode,Projection _projection = Perspective,float _fov = 60.0f,float _near = 0.01f,float _far = 15000.0f);
	~Camera() override;
public:
	void Update(float _t) override;

	void Render() override;

	void SetMovement(CameraMovementMode _mode);

	void SetProjection(Projection _projection, float _fov = 60.0f);

	inline Projection GetProjection() const { return projection; }

	inline float GetFOV() const { return fov; }

	void SetNearFar(float _near = 0.01f, float _far = 15000.0f);

	inline float GetNear() const { return near; }

	inline float GetFar() const { return far; }

	void DrawGizmo(int _color);

	CameraMovementBase* GetCameraMovement() const { return movement.get(); }
};
#endif