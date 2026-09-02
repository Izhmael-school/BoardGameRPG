#pragma once
#include "Vector/Vector3.h"
#include "../Octree/Octree.h"

/*
 * @brief 形状
 */
enum ColliderType {
	Sphere,
	Capsule,
	BoxAABB,
	BoxOBB,
	ColliderTypeMax
};

/*
 * @brief レイヤー
 */
enum CollisionLayer {
	Default,
};

/*
 * @brief AABBのデータ
 */
struct AABBShape {
	Vector3 halfExtent;	// 中心からの半幅
};

/*
 * @brief Sphereのデータ
 */
struct SphereShape {
	float radius;	// 半径
};

/*
 * @brief Capsuleのデータ
 */
struct CapsuleShape {
	Vector3 localOffset;	// スタートから終わりまでの方向と長さ
	float radius;	// 半径
};

/*
 * @brief タグ付きunion本体
 */
struct ColliderShape {
	ColliderType type;
	union {
		AABBShape aabb;
		SphereShape sphere;
		CapsuleShape capsule;
	};

	ColliderShape()
		: type(ColliderType::BoxAABB)
		, aabb{} {
	}
};

class GameObject;
class Collider;

class ColliderData {
public:
	Vector3 worldPos;		// ワールド座標
	AABB worldAABB;			// オクタツリー用
	ColliderShape shape;	// 形状データ

	CollisionLayer layer = CollisionLayer::Default;	// レイヤー
	GameObject* owner = nullptr;		// オーナー
	Collider* collider = nullptr;

	bool isEnable = true;
	bool currentHit = true;
	bool prevHit = true;
	bool isTrigger = true;			// 押し出すか
	bool isDirty = true;			// 再計算するか
};

