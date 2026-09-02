/*
 * @brief 当たり判定の生成削除管理
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
	std::vector<ColliderData> colliders;	// 実体
	std::vector<int> collidersToHandle; // 実データが何番目のハンドルか
	std::vector<int> handleToIndex;		// ハンドルから実体配列の添字
	std::vector<int> freeHandle;		// 再利用できるハンドル

	std::unique_ptr<OctreeWorld> octreeWorld;

	std::unordered_set<uint64_t> hitPairs;	// 前フレームで衝突していたペア
public:
	CollisionManager(const AABB& _worldBounds);

	/*
	 * @brief 生成
	 */
	int Create(const ColliderShape& _shape, CollisionLayer _layer, Collider* _colliderComponent);

	/*
	 * @brief 削除
	 */
	void Destroy(int _handle);

	/*
	 * @brief 更新
	 */
	void Update(float _t) override;

	/*
	 * @brief 描画
	 */
	void Render() override;

	/*
	 * @brief ワールド座標の設定
	 * @brief Transformが変化したときに呼ぶ
	 */
	void SetWorldPos(int _handle, const Vector3& _pos);

	/*
	 * @brief ハンドルから実体の添字を取得
	 */
	int GetHandleAt(int _handle) const { return collidersToHandle[_handle]; }

	/*
	 * @brief 取得
	 */
	inline ColliderData& GetCollider(int _handle) { return colliders[handleToIndex[_handle]]; }

	/*
	 * @brief すべてのコライダーを取得
	 */
	inline std::vector<ColliderData>& GetColliders() { return colliders; }

	/*
	 * @brief レイキャスト
	 */
	bool RayCast(const Vector3& _origin, const Vector3& _dir, float _dist, RayHit& _hit);

private:
	void Initialize(const AABB& _worldBounds);

	/*
	 * @brief 形状ごとのAABBを計算
	 */
	static AABB CalculateAABB(const Vector3& _worldPos, const ColliderShape& _shape);

	/*
	 * @brief hitPairからペアを取り除く
	 */
	void NotifyDestroyExit(int _handle);
};

#endif // !_COLLISIONMANAGER_H_