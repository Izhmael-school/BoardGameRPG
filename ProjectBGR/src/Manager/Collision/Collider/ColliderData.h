#pragma once
#include "Vector3.h"
#include "../Octree/Octree.h"

/*
 * @brief 蠖｢迥ｶ
 */
enum ColliderType {
	Sphere,
	Capsule,
	BoxAABB,
	BoxOBB,
	ColliderTypeMax
};

/*
 * @brief 繝ｬ繧､繝､繝ｼ
 */
enum CollisionLayer {
	Default,
};

/*
 * @brief AABB縺ｮ繝・・繧ｿ
 */
struct AABBShape {
	Vector3 halfExtent;	// 荳ｭ蠢・°繧峨・蜊雁ｹ・
};

/*
 * @brief Sphere縺ｮ繝・・繧ｿ
 */
struct SphereShape {
	float radius;	// 蜊雁ｾ・
};

/*
 * @brief Capsule縺ｮ繝・・繧ｿ
 */
struct CapsuleShape {
	Vector3 localOffset;	// 繧ｹ繧ｿ繝ｼ繝医°繧臥ｵゅｏ繧翫∪縺ｧ縺ｮ譁ｹ蜷代→髟ｷ縺・
	float radius;	// 蜊雁ｾ・
};

/*
 * @brief 繧ｿ繧ｰ莉倥″union譛ｬ菴・
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
	Vector3 worldPos;		// 繝ｯ繝ｼ繝ｫ繝牙ｺｧ讓・
	AABB worldAABB;			// 繧ｪ繧ｯ繧ｿ繝・Μ繝ｼ逕ｨ
	ColliderShape shape;	// 蠖｢迥ｶ繝・・繧ｿ

	CollisionLayer layer = CollisionLayer::Default;	// 繝ｬ繧､繝､繝ｼ
	GameObject* owner = nullptr;		// 繧ｪ繝ｼ繝翫・
	Collider* collider = nullptr;

	bool isEnable = true;
	bool currentHit = true;
	bool prevHit = true;
	bool isTrigger = true;			// 謚ｼ縺怜・縺吶°
	bool isDirty = true;			// 蜀崎ｨ育ｮ励☆繧九°
};

