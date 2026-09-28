/*
 * @brief 繧ｳ繝ｩ繧､繝繝ｼ繧ｳ繝ｳ繝昴・繝阪Φ繝・
 * @author Sekino
 */

#pragma once
#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "../ComponentBase.h"
#include "Manager/Collision/Collider/ColliderData.h"
#include "Vector3.h"

class CollisionManager;

class Collider : public ComponentBase {
private:
	CollisionManager* pManager;	// 繧ｳ繝ｩ繧､繝繝ｼ繝槭ロ繝ｼ繧ｸ繝｣繝ｼ
	int handle;					// 繧ｳ繝ｩ繧､繝繝ｼ縺ｮ繝上Φ繝峨Ν

	Vector3 lastPos;	// 蜑榊屓縺ｮ繝ｯ繝ｼ繝ｫ繝牙ｺｧ讓・

public:
	Collider(GameObject* _attachObject, CollisionManager* _colliderManager,const ColliderShape& _shape, CollisionLayer _layer);
	~Collider() override;

public:
	/*
	 * @brief 譖ｴ譁ｰ
	 */
	void Update(float _t) override;

	/*
	 * @brief 謠冗判
	 */
	void Render() override;

public:
	// 陦晉ｪ∝愛螳・
	bool IsHit() const;
	bool IsPrevHit() const;

	/*
	 * @brief 蠖薙◆縺｣縺・
	 */
	void Enter(ColliderData _pSelf, ColliderData _pOther);

	/*
	 * @brief 蠖薙◆縺｣縺ｦ繧・
	 */
	void Stay(ColliderData _pSelf, ColliderData _pOther);

	/*
	 * @brief 髮｢繧後◆
	 */
	void Exit(ColliderData _pSelf, ColliderData _pOther);

	/*
	 * @brief isActive縺ｮ繧ｻ繝・ち繝ｼ
	 */
	void SetActive(bool _isActive) override;

	/*
	 * @brief 繧ｳ繝ｩ繧､繝繝ｼ繝ｬ繧､繝､繝ｼ縺ｮ繧ｲ繝・ち繝ｼ
	 */
	CollisionLayer GetLayer() const;

	/*
	 * @brief 繧ｳ繝ｩ繧､繝繝ｼ繝ｬ繧､繝､繝ｼ縺ｮ繧ｻ繝・ち繝ｼ
	 */
	void SetLayer(CollisionLayer _layer);

	/* 
	 * @brief 螳溘ョ繝ｼ繧ｿ縺ｮ蜿門ｾ・
	 */
	ColliderData& GetColliderData() const;

	/*
	 * @brief 繝上Φ繝峨Ν縺ｮ繧ｲ繝・ち繝ｼ
	 */
	int GetHandle() const { return handle; }
};

#endif