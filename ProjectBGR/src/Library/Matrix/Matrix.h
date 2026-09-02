/*
 * @brief 行列クラス
 * @author Sekino
 */
#pragma once
#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "Library/Vector/Vector3.h"

constexpr int FOUR = 4;

class Matrix {
public:
	float m[FOUR][FOUR];

public:
	Matrix(float _0x0, float _0x1, float _0x2, float _0x3,
		   float _1x0, float _1x1, float _1x2, float _1x3,
		   float _2x0, float _2x1, float _2x2, float _2x3,
		   float _3x0, float _3x1, float _3x2, float _3x3);

public:

	/*
	 * @brief 平行移動行列の取得
	 */
	static Matrix GetTranslation(Vector3 _pos);

	/*
	 * @brief 回転Xの取得
	 */
	static Matrix GetRotationX(float xRadian);

	/*
	 * @brief 回転Yの取得
	 */
	static Matrix GetRotationY(float yRadian);

	/*
	 * @brief 回転Zの取得
	 */
	static Matrix GetRotationZ(float zRadian);

	/*
	 * @brief 回転行列の合成
	 */
	static Matrix GetRotationXYZ(Matrix _x, Matrix _y, Matrix _z);

	/*
	 * @brief 拡縮行列の取得
	 */
	static Matrix GetScale(Vector3 _scale);

	/*
	 * @brief 乗算
	 */
	static Matrix MMult(Matrix _mat1, Matrix _mat2);
};

const Matrix MZero = Matrix(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
/*
 *|1,0,0,0|
 *|0,1,0,0|
 *|0,0,1,0|
 *|0,0,0,1|
 */
const Matrix MIdentity = Matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

#endif
