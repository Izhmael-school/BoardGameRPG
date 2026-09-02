#include "CollisionManager.h"
#include "NarrowPhase/NarrowPhase.h"
#include "DxLib.h"
#include "Library/Vector/ConversionVECTOR.h"
#include "Component/Collider/Collider.h"
#include "GameObject/GameObject.h"

namespace {
	/*
	 * @brief ２つのハンドルから順序に依存しないペアキーを作る
	 */
	uint64_t MakePairKey(int _handleA, int _handleB) {
		uint32_t low = static_cast<uint32_t>(min(_handleA, _handleB));
		uint32_t high = static_cast<uint32_t>(max(_handleA, _handleB));
		return (static_cast<uint64_t>(low) << 32) | high;
	}

}

CollisionManager::CollisionManager(const AABB& _worldBounds)
	:colliders()
	, collidersToHandle()
	, handleToIndex()
	, freeHandle()
	, octreeWorld() {
	Initialize(_worldBounds);
}

int CollisionManager::Create(const ColliderShape& _shape, CollisionLayer _layer, Collider* _colliderComponent) {
	int handle;
	// 再利用できるハンドルがあればそれを使う
	if (!freeHandle.empty()) {
		handle = freeHandle.back();
		freeHandle.pop_back();
	}
	else {
		handle = (int)handleToIndex.size();
		handleToIndex.emplace_back(-1);
	}
	// 新しいコライダーを追加
	int newHandle = (int)colliders.size();
	ColliderData data;
	data.shape = _shape;
	data.layer = _layer;
	data.owner = _colliderComponent->GetAttachObject();
	data.collider = _colliderComponent;
	colliders.emplace_back(data);
	collidersToHandle.emplace_back(handle);
	handleToIndex[handle] = newHandle;
	return handle;
}

void CollisionManager::Destroy(int _handle) {
	if (handleToIndex.empty()) return;

	int index = handleToIndex[_handle];
	// ハンドルが無効なら帰る
	if (index < 0) return;

	NotifyDestroyExit(_handle);

	int lastIndex = (int)colliders.size() - 1;
	int lastHandle = collidersToHandle[lastIndex];

	// 末尾の要素を削除位置に持ってきて詰める
	colliders[index] = colliders[lastIndex];
	collidersToHandle[index] = lastHandle;
	handleToIndex[lastHandle] = index;
	colliders.pop_back();
	collidersToHandle.pop_back();
	handleToIndex[_handle] = -1;
	freeHandle.emplace_back(_handle);
}

void CollisionManager::Update(float _t) {
	if (!octreeWorld) return;

	for (auto& c : colliders) {
		// 再計算が必要でない場合はスキップ
		if (!c.isDirty || !c.isEnable) continue;
		// 形状に応じてAABBを計算
		c.worldAABB = CalculateAABB(c.worldPos, c.shape);
		c.isDirty = false;
	}

	// 前フレームのヒット状態を変更
	for (auto& c : colliders) {
		c.prevHit = c.currentHit;
		c.currentHit = false;
	}

	// オクツリーを再構築
	octreeWorld->Rebuild(*this);

	// ブロードフェーズ
	std::vector<std::pair<int, int>> pairs;
	octreeWorld->CollectAllPairs(pairs);

	// ナローフェーズ
	std::unordered_set<uint64_t> newHitPairs;
	for (auto& pair : pairs) {
		ColliderData& a = GetCollider(pair.first);
		ColliderData& b = GetCollider(pair.second);

		if (!a.isEnable || !b.isEnable) continue;

		HitResult result;
		if (!TestCollision(a, b, result)) continue;

		a.currentHit = true;
		b.currentHit = true;
		newHitPairs.insert(MakePairKey(pair.first, pair.second));

		// 押し出し
	}

	// 前フレームとの差分からイベント発火
	for (uint64_t key : newHitPairs) {
		int handleA = (int)(key >> 32);
		int handleB = (int)(key & 0xFFFFFFFFu);
		ColliderData a = GetCollider(handleA);
		ColliderData b = GetCollider(handleB);

		bool hitLastFrame = hitPairs.count(key) > 0;
		if (hitLastFrame) {
			if (a.collider)
				a.collider->Stay(a, b);
			if (b.collider)
				b.collider->Stay(a, b);
		}
		else {
			if (a.collider)
				a.collider->Enter(a, b);
			if (b.collider)
				b.collider->Enter(a, b);
		}
	}

	// 今のフレームになくなったペアはExit発火
	for (uint64_t key : hitPairs) {
		// まだ衝突しているか
		if (newHitPairs.count(key) > 0) continue;

		int handleA = (int)(key >> 32);
		int handleB = (int)(key & 0xFFFFFFFFu);

		if (handleToIndex[handleA] < 0 || handleToIndex[handleB] < 0) continue;

		ColliderData a = GetCollider(handleA);
		ColliderData b = GetCollider(handleB);

		if (a.collider)
			a.collider->Exit(a, b);
		if (b.collider)
			b.collider->Exit(a, b);
	}

	hitPairs = std::move(newHitPairs);
}

void CollisionManager::Render() {
	for (auto& c : colliders) {
		switch (c.shape.type) {
		case Sphere:
		{
			VECTOR pos = ConversionVECTOR::Vector3ToVECTOR(c.worldPos);
			DrawSphere3D(pos, c.shape.sphere.radius, 16, 0x00ff00, 0x00ff00, FALSE);
		}
		break;
		case Capsule:
		{
			CapsuleShape& cap = c.shape.capsule;
			Vector3 pos = c.worldPos;
			VECTOR start = ConversionVECTOR::Vector3ToVECTOR(pos);
			VECTOR end = ConversionVECTOR::Vector3ToVECTOR(Vector3::VAdd(c.worldPos, cap.localOffset));
			DrawCapsule3D(start, end, cap.radius, 16, 0x00ff00, 0x00ff00, FALSE);
		}
		break;
		case BoxAABB:
		{
			AABBShape& box = c.shape.aabb;
			Vector3 pos = c.worldPos;
			Vector3 half = box.halfExtent;
			VECTOR pos1 = ConversionVECTOR::Vector3ToVECTOR(Vector3::VAdd(pos, half));
			VECTOR pos2 = ConversionVECTOR::Vector3ToVECTOR(Vector3::VSub(pos, half));
			DrawCube3D(pos1, pos2, 0x00ff00, 0x00ff00, FALSE);
		}
		break;
		case BoxOBB:
			break;
		}
	}
}

void CollisionManager::SetWorldPos(int _handle, const Vector3& _pos) {
	ColliderData& c = GetCollider(_handle);
	c.worldPos = _pos;
	c.isDirty = true;
}

bool CollisionManager::RayCast(const Vector3& _origin, const Vector3& _dir, float _dist, RayHit& _hit) {
	if (!octreeWorld) return false;
	return false;
}

void CollisionManager::Initialize(const AABB& _worldBounds) {
	static bool narrowPhaseInit = false;

	if (narrowPhaseInit)return;

	NarrowPhaseInitialize();
	narrowPhaseInit = true;

	octreeWorld = std::make_unique<OctreeWorld>(_worldBounds);
}

AABB CollisionManager::CalculateAABB(const Vector3& _worldPos, const ColliderShape& _shape) {
	switch (_shape.type) {
	case ColliderType::BoxAABB:
		// BoxのAABBを計算
		return AABB(_worldPos, _shape.aabb.halfExtent);
	case ColliderType::Sphere:
		// SphereのAABBを計算
		return AABB(_worldPos, Vector3::VScale(VOne, _shape.sphere.radius));
	case ColliderType::Capsule:
		// CapsuleのAABBを計算
		const Vector3& offset = _shape.capsule.localOffset;
		float radius = _shape.capsule.radius;

		Vector3 start = _worldPos;
		Vector3 end = Vector3::VAdd(_worldPos, offset);

		float minX = min(start.x, end.x) - radius;
		float maxX = max(start.x, end.x) + radius;
		float minY = min(start.y, end.y) - radius;
		float maxY = max(start.y, end.y) + radius;
		float minZ = min(start.z, end.z) - radius;
		float maxZ = max(start.z, end.z) + radius;

		Vector3 center = Vector3((minX + maxX) * 0.5f, (minY + maxY) * 0.5f, (minZ + maxZ) * 0.5f);
		Vector3 halfSize = Vector3((maxX - minX) * 0.5f, (maxY - minY) * 0.5f, (maxZ - minZ) * 0.5f);

		return AABB(center, halfSize);
	}
	return AABB();
}

void CollisionManager::NotifyDestroyExit(int _handle) {
	Collider* selfComponent = GetCollider(_handle).collider;

	for (auto itr = hitPairs.begin(); itr != hitPairs.end();) {
		int handleA = (int)(*itr >> 32);
		int handleB = (int)(*itr & 0xFFFFFFFFu);

		// 関係ないペアはスキップ
		if (handleA != _handle && handleB != _handle) {
			itr++;
			continue;
		}

		int otherHandle = (handleA == _handle) ? handleB : handleA;
		Collider* otherComponent = (handleToIndex[otherHandle] >= 0) ? GetCollider(otherHandle).collider : nullptr;

		// 相手がまだいればExitに入る
		otherComponent->Exit(otherComponent->GetColliderData(), selfComponent->GetColliderData());
		selfComponent->Exit(selfComponent->GetColliderData(), otherComponent->GetColliderData());

		itr = hitPairs.erase(itr);
	}
}
