/*
 * @brief 蠖薙◆繧雁愛螳壹・逕滓・蜑企勁邂｡逅・
 * @author Sekino
 */
#pragma once
#ifndef _COLLISIONMANAGER_H_
#define _COLLISIONMANAGER_H_

#include "../ManagerBase.h"
#include <vector>
#include <memory>
#include <unordered_set>
#include <cstdint>
#include "Collider/ColliderData.h"

class GameObject;

struct RayHit {
	int handle = -1;
	GameObject* object = nullptr;
	Vector3 point;
	Vector3 normal;
	float dist = 0.0f;
};

class CollisionManager : public ManagerBase {
private:
	std::vector<ColliderData> colliders;	// 螳滉ｽ・
	std::vector<int> collidersToHandle; // 螳溘ョ繝ｼ繧ｿ縺御ｽ慕分逶ｮ縺ｮ繝上Φ繝峨Ν縺・
	std::vector<int> handleToIndex;		// 繝上Φ繝峨Ν縺九ｉ螳滉ｽ馴・蛻励・豺ｻ蟄・
	std::vector<int> freeHandle;		// 蜀榊茜逕ｨ縺ｧ縺阪ｋ繝上Φ繝峨Ν

	std::unique_ptr<OctreeWorld> octreeWorld;

	std::unordered_set<uint64_t> hitPairs;	// 蜑阪ヵ繝ｬ繝ｼ繝縺ｧ陦晉ｪ√＠縺ｦ縺・◆繝壹い
public:
	CollisionManager(const AABB& _worldBounds);

	/*
	 * @brief 逕滓・
	 */
	int Create(const ColliderShape& _shape, CollisionLayer _layer, Collider* _colliderComponent);

	/*
	 * @brief 蜑企勁
	 */
	void Destroy(int _handle);

	/*
	 * @brief 譖ｴ譁ｰ
	 */
	void Update(float _t) override;

	/*
	 * @brief 謠冗判
	 */
	void Render() override;

	/*
	 * @brief 繝ｯ繝ｼ繝ｫ繝牙ｺｧ讓吶・險ｭ螳・
	 * @brief Transform縺悟､牙喧縺励◆縺ｨ縺阪↓蜻ｼ縺ｶ
	 */
	void SetWorldPos(int _handle, const Vector3& _pos);

	/*
	 * @brief 繝上Φ繝峨Ν縺九ｉ螳滉ｽ薙・豺ｻ蟄励ｒ蜿門ｾ・
	 */
	int GetHandleAt(int _handle) const { return collidersToHandle[_handle]; }

	/*
	 * @brief 蜿門ｾ・
	 */
	inline ColliderData& GetCollider(int _handle) { return colliders[handleToIndex[_handle]]; }

	/*
	 * @brief 縺吶∋縺ｦ縺ｮ繧ｳ繝ｩ繧､繝繝ｼ繧貞叙蠕・
	 */
	inline std::vector<ColliderData>& GetColliders() { return colliders; }

	/*
	 * @brief 繝ｬ繧､繧ｭ繝｣繧ｹ繝・
	 */
	bool RayCast(const Vector3& _origin, const Vector3& _dir, float _dist, RayHit& _hit);

private:
	void Initialize(const AABB& _worldBounds);

	/*
	 * @brief 蠖｢迥ｶ縺斐→縺ｮAABB繧定ｨ育ｮ・
	 */
	static AABB CalculateAABB(const Vector3& _worldPos, const ColliderShape& _shape);

	/*
	 * @brief hitPair縺九ｉ繝壹い繧貞叙繧企勁縺・
	 */
	void NotifyDestroyExit(int _handle);
};

#endif // !_COLLISIONMANAGER_H_