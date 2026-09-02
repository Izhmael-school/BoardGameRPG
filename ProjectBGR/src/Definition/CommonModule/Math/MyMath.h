/*
 * @brief 計算関数の寄せ集め
 * @author Sekino
 */
#pragma once
#ifndef _MYMATH_H_
#define _MYMATH_H_

class MyMath {
public:
	/*
	 * @brief 弧度法から度数法
	 */
	static float Rad2Deg(float _degree);

	/*
	 * @brief 度数法から弧度法
	 */
	static float Deg2Rad(float _radian);

	/*
	 * @brief 整数の乱数
	 */
	static int Random(int min, int max);
};
#endif