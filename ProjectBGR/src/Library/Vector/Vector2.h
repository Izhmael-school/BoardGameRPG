/*
 * @brief 2方向のベクトルクラス
 */
#pragma once
#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <array>
#include <cassert>

class Vector3;

class Vector2 {
public:
	float x, y;

public:
	Vector2();
	Vector2(float _x, float _y);

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
	Vector2 Normalized() const;

	/*
	 * @brief 繝吶け繝医Ν縺ｮ髟ｷ縺輔・2荵励ｒ霑斐☆
	 */
	static float SqrMagnitude(Vector2 _vec);

	/*
	 * @brief 繝吶け繝医Ν縺ｮ髟ｷ縺輔・2荵励ｒ霑斐☆
	 */
	static float SqrMagnitude(float _x, float _y, float _z);

	/*
	 * @brief 隕∫ｴ繧帝・蛻励→縺励※霑斐☆
	 */
	std::array<float, 2> GetArray() const;

	/*
	 * @brief 2轤ｹ髢薙・隗貞ｺｦ繧定ｿ斐☆
	 */
	static Vector2 Angle(Vector2 _from, Vector2 _to);

	/*
	 * @brief 2轤ｹ縺ｮ蜀・ｩ阪ｒ霑斐☆
	 */
	static float Dot(Vector2 _vec1, Vector2 _vec2);

	/*
	 * @brief 2轤ｹ髢薙・霍晞屬繧定ｿ斐☆
	 */
	static float Distance(Vector2 _vec1, Vector2 _vec2);

	/*
	 * @brief 逶ｴ邱壻ｸ翫↓縺ゅｋ2轤ｹ縺ｮ陬憺俣繧定ｿ斐☆
	 */
	static Vector2 Lerp(Vector2 _vec1, Vector2 _vec2, float _t);

	/*
	 * @brief 2轤ｹ縺ｮ譛螟ｧ轤ｹ繧貞叙蠕・
	 */
	static Vector2 Max(Vector2 _vec1, Vector2 _vec2);

	/*
	 * @brief 2轤ｹ縺ｮ譛螟ｧ轤ｹ繧貞叙蠕・
	 */
	static Vector2 Min(Vector2 _vec1, Vector2 _vec2);

	/*
	 * @brief 蜉邂・
	 */
	static Vector2 VAdd(Vector2 _vec1, Vector2 _vec2);

	/*
	 * @brief 蜉邂・
	 */
	static Vector2 VAdd(Vector2 _vec, float _value);

	/*
	 * @brief 貂帷ｮ・
	 */
	static Vector2 VSub(Vector2 _vec1, Vector2 _vec2);

	/*
	 * @brief 貂帷ｮ・
	 */
	static Vector2 VSub(Vector2 _vec, float _value);

	/*
	 * @brief 荵礼ｮ・
	 */
	static Vector2 VMult(Vector2 _vec1, Vector2 _vec2);

	/*
	 * @brief 繝吶け繝医Ν縺ｮ蜷・ｦ∫ｴ縺ｨ荵礼ｮ・
	 */
	static Vector2 VScale(Vector2 _vec, float _scale);

	float& operator[](size_t i) {
		assert(i < 2);
		if (i == 0) return x;
		return y;
	}

	const float& operator[](size_t i) const {
		assert(i < 2);
		if (i == 0) return x;
		if (i == 1) return y;

	}

	Vector2 operator+(const Vector3& _v) const; 
	Vector2 operator+=(const Vector3& _v) const;
	Vector2 operator-(const Vector3& _v) const;
	Vector2 operator-=(const Vector3& _v) const; 
	Vector2 operator*(const Vector3& _v) const;
	Vector2 operator*=(const Vector3& _v) const;
	Vector2 operator=(const Vector3& _v) const;
	bool operator==(const Vector3& _v) const;

	Vector2 operator+(const Vector2& _v) const { return { x + _v.x,y + _v.y }; }
	Vector2 operator+=(const Vector2& _v) const { return { x + _v.x,y + _v.y }; }
	Vector2 operator-(const Vector2& _v) const { return { x - _v.x,y - _v.y }; }
	Vector2 operator-=(const Vector2& _v) const { return { x - _v.x,y - _v.y }; }
	Vector2 operator*(const Vector2& _v) const { return { x * _v.x,y * _v.y }; }
	Vector2 operator*=(const Vector2& _v) const { return { x * _v.x,y * _v.y }; }
	bool operator==(const Vector2& _v) const { return { x == _v.x && y == _v.y }; }
};

const Vector2 VZero_2 = Vector2(0.0f, 0.0f);	// (0,0)
const Vector2 VOne_2 = Vector2(1.0f, 1.0f);		// (1,1)
const Vector2 VRight_2 = Vector2(1.0f, 0.0f);	// (1,0)
const Vector2 VLeft_2 = Vector2(-1.0f, 0.0f);	// (-1,0)
const Vector2 VUp_2 = Vector2(0.0f, 1.0f);		// (0,1)
const Vector2 VDown_2 = Vector2(0.0f, -1.0f);	// (0,-1)

#endif