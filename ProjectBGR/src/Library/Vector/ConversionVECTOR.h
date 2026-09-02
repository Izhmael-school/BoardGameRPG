/*
 * @brief DxライブラリのVECTORとオリジナルのVector3を変換する
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
	 * @brief Vector3をVECTORに変換
	 */
	static VECTOR Vector3ToVECTOR(Vector3 _vec);

	/*
	 * @brief VECTORをVector3に変換
	 */
	static Vector3 VECTORToVector3(VECTOR _vec);
};

#endif