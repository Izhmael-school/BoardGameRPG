/*
 * @brief Dx繝ｩ繧､繝悶Λ繝ｪ縺ｮVECTOR縺ｨ繧ｪ繝ｪ繧ｸ繝翫Ν縺ｮVector3繧貞､画鋤縺吶ｋ
 * @author Sekino
 */
#pragma once
#ifndef _CONVERSIONVECTOR_H_
#define _CONVERSIONVECTOR_H_

#include "DxLib.h"
#include "Vector3.h"

class ConversionVECTOR {
public:
	/*
	 * @brief Vector3繧歎ECTOR縺ｫ螟画鋤
	 */
	static VECTOR Vector3ToVECTOR(Vector3 _vec);

	/*
	 * @brief VECTOR繧歎ector3縺ｫ螟画鋤
	 */
	static Vector3 VECTORToVector3(VECTOR _vec);
};

#endif