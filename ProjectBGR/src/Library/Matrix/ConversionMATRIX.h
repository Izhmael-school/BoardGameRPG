/*
 * @brief Dx繝ｩ繧､繝悶Λ繝ｪ縺ｮMATRIX縺ｨ繧ｪ繝ｪ繧ｸ繝翫Ν縺ｮMatrix繧貞､画鋤縺吶ｋ
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
	 * @brief Matrix繧樽ATRIX縺ｫ螟画鋤
	 */
	static MATRIX MatrixToMATRIX(Matrix _mat);

	/*
	 * @brief MATRIX繧樽atrix縺ｫ螟画鋤
	 */
	static Matrix MATRIXToMatrix(MATRIX _mat);
};

#endif