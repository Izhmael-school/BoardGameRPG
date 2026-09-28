/*
 * @brief 三方向のベクトルクラス
 * @author Sekino
 */
#pragma once
#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <array>
#include <cassert>

class Vector2;

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
	 * @brief 繝吶け繝医Ν縺ｮ髟ｷ縺輔・2荵励ｒ霑斐☆
	 */
	float SqrMagnitude() const;


	/*
	 * @brief 豁｣隕丞喧縺励◆繝吶け繝医Ν繧定ｿ斐☆
	 */
	Vector3 Normalized() const;

	/*
	 * @brief 繝吶け繝医Ν縺ｮ髟ｷ縺輔・2荵励ｒ霑斐☆
	 */
	static float SqrMagnitude(Vector3 _vec);

	/*
	 * @brief 繝吶け繝医Ν縺ｮ髟ｷ縺輔・2荵励ｒ霑斐☆
	 */
	static float SqrMagnitude(float _x, float _y, float _z);

	/*
	 * @brief 隕∫ｴ繧帝・蛻励→縺励※霑斐☆
	 */
	std::array<float, 3> GetArray() const;

	/*
	 * @brief 2轤ｹ髢薙・隗貞ｺｦ繧定ｿ斐☆
	 */
	static Vector3 Angle(Vector3 _from, Vector3 _to);

	/*
	 * @brief 2轤ｹ縺ｮ蜀・ｩ阪ｒ霑斐☆
	 */
	static float Dot(Vector3 _vec1, Vector3 _vec2);

	/*
	 * @brief 2轤ｹ縺ｮ螟也ｩ阪ｒ霑斐☆
	 */
	static Vector3 Cross(Vector3 _vec1, Vector3 _vec2);

	/*
	 * @brief 2轤ｹ髢薙・霍晞屬繧定ｿ斐☆
	 */
	static float Distance(Vector3 _vec1, Vector3 _vec2);

	/*
	 * @brief 逶ｴ邱壻ｸ翫↓縺ゅｋ2轤ｹ縺ｮ陬憺俣繧定ｿ斐☆
	 */
	static Vector3 Lerp(Vector3 _vec1, Vector3 _vec2, float _t);

	/*
	 * @brief 2轤ｹ縺ｮ譛螟ｧ轤ｹ繧貞叙蠕・
	 */
	static Vector3 Max(Vector3 _vec1, Vector3 _vec2);

	/*
	 * @brief 2轤ｹ縺ｮ譛螟ｧ轤ｹ繧貞叙蠕・
	 */
	static Vector3 Min(Vector3 _vec1, Vector3 _vec2);

	/*
	 * @brief 蜉邂・
	 */
	static Vector3 VAdd(Vector3 _vec1, Vector3 _vec2);

	/*
	 * @brief 蜉邂・
	 */
	static Vector3 VAdd(Vector3 _vec, float _value);

	/*
	 * @brief 貂帷ｮ・
	 */
	static Vector3 VSub(Vector3 _vec1, Vector3 _vec2);

	/*
	 * @brief 貂帷ｮ・
	 */
	static Vector3 VSub(Vector3 _vec, float _value);

	/*
	 * @brief 荵礼ｮ・
	 */
	static Vector3 VMult(Vector3 _vec1, Vector3 _vec2);

	/*
	 * @brief 繝吶け繝医Ν縺ｮ蜷・ｦ∫ｴ縺ｨ荵礼ｮ・
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

	Vector3 operator+(const Vector3& _v) const { return { x + _v.x,y + _v.y,z + _v.z }; }
	Vector3 operator+=(const Vector3& _v) const { return { x + _v.x,y + _v.y,z + _v.z }; }
	Vector3 operator-(const Vector3& _v) const { return { x - _v.x,y - _v.y,z - _v.z }; }
	Vector3 operator-=(const Vector3& _v) const { return { x - _v.x,y - _v.y,z - _v.z }; }
	Vector3 operator*(const Vector3& _v) const { return { x * _v.x,y * _v.y,z * _v.z }; }
	Vector3 operator*=(const Vector3& _v) const { return { x * _v.x,y * _v.y,z * _v.z }; }
	bool operator==(const Vector3& _v) const { return { x == _v.x && y == _v.y && z == _v.z }; }

	Vector3 operator+(const Vector2& _v) const;
	Vector3 operator+=(const Vector2& _v) const;
	Vector3 operator-(const Vector2& _v) const;
	Vector3 operator-=(const Vector2& _v) const;
	Vector3 operator*(const Vector2& _v) const;
	Vector3 operator*=(const Vector2& _v) const;
	Vector3 operator=(const Vector2& _v) const;
	bool operator==(const Vector2& _v) const;
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