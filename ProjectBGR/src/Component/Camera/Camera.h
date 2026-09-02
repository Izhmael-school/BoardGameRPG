/*
 * @brief カメラコンポーネント
 * @author Sekino
 */
#pragma once
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../ComponentBase.h"
#include <memory>

class CameraMovementBase;

enum Projection {
	Perspective,	// 立体に写す
	Orthographic	// 平面に写す
};

enum CameraMovementMode {
	Free,
};

class Camera : public ComponentBase {
private:
	Projection projection;	// 描画方法
	float fov;				// 視野角
	float near;				// 手前クリップの距離
	float far;				// 奥グリップの距離

	std::unique_ptr<CameraMovementBase> movement;

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
};
#endif