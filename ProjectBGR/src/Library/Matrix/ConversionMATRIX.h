/*
 * @brief DxライブラリのMATRIXとオリジナルのMatrixを変換する
 * @author Sekino
 */
#pragma once
#ifndef _CONVERSIONMATRIX_H_
#define _CONVERSIONMATRIX_H_

#include "DxLib.h"
#include "Matrix.h"

class ConversionMATRIX {
public:
	/*
	 * @brief MatrixをMATRIXに変換
	 */
	static MATRIX MatrixToMATRIX(Matrix _mat);

	/*
	 * @brief MATRIXをMatrixに変換
	 */
	static Matrix MATRIXToMatrix(MATRIX _mat);
};

#endif