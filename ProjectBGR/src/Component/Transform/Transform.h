/*
 * @file Transform.h
 * @author Sekino
 */
#pragma once
#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <vector>
#include "Vector3.h"
#include "Matrix.h"
#include "../ComponentBase.h"

class Transform : public ComponentBase{
protected:
	// 座標
	Vector3 position;
	// 回転(オイラー角)
	Vector3 rotation;
	// 拡縮
	Vector3 scale;
	// 行列
	Matrix matrix;
	// 親子関係
	Transform* parent;
	std::vector<Transform*> children;

	// 有効かどうか
	bool isActive;

public:
	Transform(GameObject* _attachObject);
	~Transform();

	/*
	 * @brief 更新
	 */
	void Update(float _t) override;

	// 座標関連
	inline Vector3 GetPosition() const { return Vector3(matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]); }
	inline Vector3 GetLocalPosition() const { return position; }
	inline void SetPosition(Vector3 _pos) { position = _pos; CalcMatrix(); }
	inline void AddPosition(Vector3 _add) { position = Vector3::VAdd(position, _add); CalcMatrix();}
	inline void AddPosition(Vector3 _dir, float _add) { position = Vector3::VAdd(position, Vector3::VScale(_dir, _add)); CalcMatrix();}

	// 回転関連
	Vector3 GetRotation();
	inline Vector3 GetLocalRotation() const { return rotation; }
	inline void SetRotation(Vector3 _rot) { rotation = _rot;CalcMatrix();}
	inline void AddRotation(Vector3 _add) { rotation = Vector3::VAdd(rotation, _add); CalcMatrix();}
	inline void AddRotation(Vector3 _dir, float _add) { rotation = Vector3::VAdd(rotation, Vector3::VScale(_dir, _add));CalcMatrix();}

	// 拡縮関連
	Vector3 GetScale() const;
	inline Vector3 GetLocalScale() const { return scale; }
	inline void SetScale(Vector3 _sca) { scale = _sca; CalcMatrix(); }
	inline void SetScale(float _sca) { scale = Vector3(_sca, _sca, _sca); CalcMatrix(); }
	inline void AddScale(Vector3 _add) { scale = Vector3::VAdd(scale, _add); CalcMatrix(); }
	inline void AddScale(Vector3 _dir, float _add) { scale = Vector3::VAdd(scale, Vector3::VScale(_dir, _add)); CalcMatrix(); }

	// 行列関連
	inline Matrix GetMatrix() const { return matrix; }
	inline void SetMatrix(Matrix _mat) { matrix = _mat; }
	Matrix CalcMatrix();
	void CalcTransform();

	// ベクトル関連
	inline Vector3 GetForward() { return Vector3(matrix.m[2][0], matrix.m[2][1], matrix.m[2][2]).Normalized(); }
	inline Vector3 GetUp() const { return Vector3(matrix.m[1][0], matrix.m[1][1], matrix.m[1][2]).Normalized(); }
	inline Vector3 GetRight() const { return Vector3(matrix.m[0][0], matrix.m[0][1], matrix.m[0][2]).Normalized(); }

	void LookAtPos(Vector3 targetPos);
	void LookAtDir(Vector3 dir);
	/*
	 *	ターゲットの方を向く
	 *	@author	Riku
	 */
	void LookAt(Vector3 targetPos);

	void GraduallyLookAtY(Vector3 targetPos);

	// 親子関係関連
	/*
	 * @brief 親子関係を作る
	 * @param _parant 親になるTransform
	 * @param isHoldWorld 現在の座標を維持するか
	 */
	void AttachParent(Transform* _parent, bool isHoldWorld = true);
	/*
	 * @brief 親子関係を解除する
	 */
	void DetachParent();

	Transform* GetParent() const { return parent; }
	Transform* GetChild(int index) const { if (children.size() <= index) return nullptr; else return children[index]; }
	int GetChildCount() const { return static_cast<int>(children.size()); }
	/*
	 * @brief 子供としてのIDを取得する
	 */
	int GetChildID();
};
#endif