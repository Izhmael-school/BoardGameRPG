/*
 * @brief コライダーコンポーネント
 * @author Sekino
 */

#pragma once
#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "../ComponentBase.h"
#include "Manager/Collision/Collider/ColliderData.h"
#include "Vector/Vector3.h"

class CollisionManager;

class Collider : public ComponentBase {
private:
	CollisionManager* pManager;	// コライダーマネージャー
	int handle;					// コライダーのハンドル

	Vector3 lastPos;	// 前回のワールド座標

public:
	Collider(GameObject* _attachObject, CollisionManager* _colliderManager,const ColliderShape& _shape, CollisionLayer _layer);
	~Collider() override;

public:
	/*
	 * @brief 更新
	 */
	void Update(float _t) override;

	/*
	 * @brief 描画
	 */
	void Render() override;

public:
	// 衝突判定
	bool IsHit() const;
	bool IsPrevHit() const;

	/*
	 * @brief 当たった
	 */
	void Enter(ColliderData _pSelf, ColliderData _pOther);

	/*
	 * @brief 当たってる
	 */
	void Stay(ColliderData _pSelf, ColliderData _pOther);

	/*
	 * @brief 離れた
	 */
	void Exit(ColliderData _pSelf, ColliderData _pOther);

	/*
	 * @brief isActiveのセッター
	 */
	void SetActive(bool _isActive) override;

	/*
	 * @brief コライダーレイヤーのゲッター
	 */
	CollisionLayer GetLayer() const;

	/*
	 * @brief コライダーレイヤーのセッター
	 */
	void SetLayer(CollisionLayer _layer);

	/* 
	 * @brief 実データの取得
	 */
	ColliderData& GetColliderData() const;

	/*
	 * @brief ハンドルのゲッター
	 */
	int GetHandle() const { return handle; }
};

#endif