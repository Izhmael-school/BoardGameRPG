/*
 * @brief ベクトルクラス
 * @author Sekino
 */
#pragma once
#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <array>
#include <cassert>

class Vector3 {
public:
	float x, y, z;

public:
	Vector3();
	Vector3(float _x, float _y, float _z = 0.0f);

	/*
	 * @brief ベクトルの長さを返す
	 */
	float Magnitude() const;

	/*
	 * @brief ベクトルの長さの2乗を返す
	 */
	float SqrMagnitude() const;


	/*
	 * @brief 正規化したベクトルを返す
	 */
	Vector3 Normalized() const;

	/*
	 * @brief ベクトルの長さの2乗を返す
	 */
	static float SqrMagnitude(Vector3 _vec);

	/*
	 * @brief ベクトルの長さの2乗を返す
	 */
	static float SqrMagnitude(float _x, float _y, float _z);

	/*
	 * @brief 要素を配列として返す
	 */
	std::array<float, 3> GetArray() const;

	/*
	 * @brief 2点間の角度を返す
	 */
	static Vector3 Angle(Vector3 _from, Vector3 _to);

	/*
	 * @brief 2点の内積を返す
	 */
	static float Dot(Vector3 _vec1, Vector3 _vec2);

	/*
	 * @brief 2点の外積を返す
	 */
	static Vector3 Cross(Vector3 _vec1, Vector3 _vec2);

	/*
	 * @brief 2点間の距離を返す
	 */
	static float Distance(Vector3 _vec1, Vector3 _vec2);

	/*
	 * @brief 直線上にある2点の補間を返す
	 */
	static Vector3 Lerp(Vector3 _vec1, Vector3 _vec2, float _t);

	/*
	 * @brief 2点の最大点を取得
	 */
	static Vector3 Max(Vector3 _vec1, Vector3 _vec2);

	/*
	 * @brief 2点の最大点を取得
	 */
	static Vector3 Min(Vector3 _vec1, Vector3 _vec2);

	/*
	 * @brief 加算
	 */
	static Vector3 VAdd(Vector3 _vec1, Vector3 _vec2);

	/*
	 * @brief 加算
	 */
	static Vector3 VAdd(Vector3 _vec, float _value);

	/*
	 * @brief 減算
	 */
	static Vector3 VSub(Vector3 _vec1, Vector3 _vec2);

	/*
	 * @brief 減算
	 */
	static Vector3 VSub(Vector3 _vec, float _value);

	/*
	 * @brief 乗算
	 */
	static Vector3 VMult(Vector3 _vec1, Vector3 _vec2);

	/*
	 * @brief ベクトルの各要素と乗算
	 */
	static Vector3 VScale(Vector3 _vec, float _scale);

	float& operator[](size_t i) {
		assert(i < 3);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}

	const float& operator[](size_t i) const {
		assert(i < 3);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
};

const Vector3 VZero = Vector3(0.0f, 0.0f, 0.0f);	// (0,0,0)
const Vector3 VOne = Vector3(1.0f, 1.0f, 1.0f);		// (1,1,1)
const Vector3 VRight = Vector3(1.0f, 0.0f, 0.0f);	// (1,0,0)
const Vector3 VLeft = Vector3(-1.0f, 0.0f, 0.0f);	// (-1,0,0)
const Vector3 VUp = Vector3(0.0f, 1.0f, 0.0f);		// (0,1,0)
const Vector3 VDown = Vector3(0.0f, -1.0f, 0.0f);	// (0,-1,0)
const Vector3 VForward = Vector3(0.0f, 0.0f, 1.0f);	// (0,0,1)
const Vector3 VBack = Vector3(0.0f, 0.0f, -1.0f);	// (0,0,-1)

#endif // !_VECTOR3_H_