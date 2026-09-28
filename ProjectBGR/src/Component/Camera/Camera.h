/*
 * @brief 繧ｫ繝｡繝ｩ繧ｳ繝ｳ繝昴・繝阪Φ繝・
 * @author Sekino
 */
#pragma once
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../ComponentBase.h"
#include <memory>

class CameraMovementBase;

enum Projection {
	Perspective,	// 遶倶ｽ薙↓蜀吶☆
	Orthographic	// 蟷ｳ髱｢縺ｫ蜀吶☆
};

enum CameraMovementMode {
	Free,
};

class Camera : public ComponentBase {
private:
	Projection projection;	// 謠冗判譁ｹ豕・
	float fov;				// 隕夜㍽隗・
	float near;				// 謇句燕繧ｯ繝ｪ繝・・縺ｮ霍晞屬
	float far;				// 螂･繧ｰ繝ｪ繝・・縺ｮ霍晞屬

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