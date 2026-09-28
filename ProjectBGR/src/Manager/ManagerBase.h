/*
 * @brief 邂｡逅・け繝ｩ繧ｹ縺ｮ蝓ｺ蠎・
 * @author Sekino
 */
#pragma once
#ifndef _MANAGERBASE_H_
#define _MANAGERBASE_H_

class ManagerBase{
public:
	ManagerBase() = default;
	virtual ~ManagerBase() = default;

protected:
	/*
	 * @brief 逕滓・譎ゅ↓蜈･繧具ｼ医Μ繧ｽ繝ｼ繧ｹ繝ｭ繝ｼ繝臥ｭ会ｼ・
	 */
	virtual void Start();

public:
	
	/*
	 * @brief 譖ｴ譁ｰ
	 */
	virtual void Update(float _t);

	/*
	 * @brief 謠冗判
	 */
	virtual void Render();

	/*
	 * @brief 蛻晄悄蛹・
	 */
	virtual void Setup();

	/*
	 * @brief 蠕悟・逅・
	 */
	virtual void Cleanup();
};
#endif