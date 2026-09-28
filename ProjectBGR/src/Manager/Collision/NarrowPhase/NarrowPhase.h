/*
 * @brief 邊ｾ蟇・↑蛻､螳壹・縺薙％縺ｧ陦後≧
 * @author Sekino
 */
#pragma once
#ifndef _NARROWPHASE_H_
#define _NARROWPHASE_H_

#include "Vector3.h"
#include "../Collider/ColliderData.h"

/*
 * @brief 陦晉ｪ∝愛螳壹・邨先棡
 */
struct HitResult {
	Vector3 normal;	// 豕慕ｷ・
	Vector3 point;	// 陦晉ｪ∫せ
	float penetration = 0.0f;	// 繧√ｊ霎ｼ縺ｿ驥・
};

/*
 * @brief 陦晉ｪ∝愛螳夐未謨ｰ縺ｮ蝙・
 */
using NarrowPhaseFunc = bool(*)(const ColliderData& _a, const ColliderData& _b, HitResult& _result);

/*
 * @brief 陦晉ｪ∝愛螳壹・蛻晄悄蛹・
 */
void NarrowPhaseInitialize();

/*
 * @brief 陦晉ｪ∝愛螳夐未謨ｰ縺ｮ蜿門ｾ・
 */
NarrowPhaseFunc GetNarrowPhaseFunc(ColliderType _typeA,ColliderType _typeB);

// ------------------------------------------------------------
// 陦晉ｪ∝愛螳夐未謨ｰ縺ｮ螳｣險
// ------------------------------------------------------------

bool TestCollision(const ColliderData& _a, const ColliderData& _b, HitResult& _result);

bool TestSphereSphere(const ColliderData& _a, const ColliderData& _b, HitResult& _result);
bool TestSphereCapsule(const ColliderData& _sphere, const ColliderData& _capsule, HitResult& _result);
bool TestSphereAABB(const ColliderData& _sphere, const ColliderData& _aabb, HitResult& _result);
bool TestCapsuleCapsule(const ColliderData& _a, const ColliderData& _b, HitResult& _result);
bool TestCapsuleAABB(const ColliderData& _capsule, const ColliderData& _aabb, HitResult& _result);
bool TestAABBAABB(const ColliderData& _a, const ColliderData& _b, HitResult& _result);

/*
 * @brief 繝ｬ繧､縺ｨ繧ｳ繝ｩ繧､繝繝ｼ縺ｮ莠､蟾ｮ蛻､螳・
 */
bool RayVsCollider(const Vector3& _origin, const Vector3& _dir, const ColliderData _collider, float _dist, float& hitDist, Vector3& _hitNormal);
#endif 