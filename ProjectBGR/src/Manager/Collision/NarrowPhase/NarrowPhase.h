/*
 * @brief 精密な判定はここで行う
 * @author Sekino
 */
#pragma once
#ifndef _NARROWPHASE_H_
#define _NARROWPHASE_H_

#include "Vector/Vector3.h"
#include "../Collider/ColliderData.h"

/*
 * @brief 衝突判定の結果
 */
struct HitResult {
	Vector3 normal;	// 法線
	Vector3 point;	// 衝突点
	float penetration = 0.0f;	// めり込み量
};

/*
 * @brief 衝突判定関数の型
 */
using NarrowPhaseFunc = bool(*)(const ColliderData& _a, const ColliderData& _b, HitResult& _result);

/*
 * @brief 衝突判定の初期化
 */
void NarrowPhaseInitialize();

/*
 * @brief 衝突判定関数の取得
 */
NarrowPhaseFunc GetNarrowPhaseFunc(ColliderType _typeA,ColliderType _typeB);

// ------------------------------------------------------------
// 衝突判定関数の宣言
// ------------------------------------------------------------

bool TestCollision(const ColliderData& _a, const ColliderData& _b, HitResult& _result);

bool TestSphereSphere(const ColliderData& _a, const ColliderData& _b, HitResult& _result);
bool TestSphereCapsule(const ColliderData& _sphere, const ColliderData& _capsule, HitResult& _result);
bool TestSphereAABB(const ColliderData& _sphere, const ColliderData& _aabb, HitResult& _result);
bool TestCapsuleCapsule(const ColliderData& _a, const ColliderData& _b, HitResult& _result);
bool TestCapsuleAABB(const ColliderData& _capsule, const ColliderData& _aabb, HitResult& _result);
bool TestAABBAABB(const ColliderData& _a, const ColliderData& _b, HitResult& _result);

/*
 * @brief レイとコライダーの交差判定
 */
bool RayVsCollider(const Vector3& _origin, const Vector3& _dir, const ColliderData _collider, float _dist, float& hitDist, Vector3& _hitNormal);
#endif 