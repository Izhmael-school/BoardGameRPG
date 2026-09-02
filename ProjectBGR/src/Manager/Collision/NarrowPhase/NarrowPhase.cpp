#include "NarrowPhase.h"
#include <algorithm>
#include <cmath>

namespace {
	constexpr float EPSILON = 1e-6f;

	/*
	 * @brief 衝突判定関数のテーブル
	 */
	NarrowPhaseFunc table[ColliderType::ColliderTypeMax][ColliderType::ColliderTypeMax] = {};

	/*
	 * @brief 衝突判定関数の引数を入れ替えて呼ぶ
	 */
	template<NarrowPhaseFunc Func>
	bool Swap(const ColliderData& _a, const ColliderData& _b, HitResult& _result) {
		if (!Func(_b, _a, _result))
			return false;

		// 法線の向きを反転させる
		_result.normal = Vector3(-_result.normal.x, -_result.normal.y, -_result.normal.z);

		return true;
	}

	/*
	 * @brief 線分上の最近点を求める
	 */
	Vector3 ClosestPointOnSegment(const Vector3& _point, const Vector3& _start, const Vector3& _end) {
		Vector3 segment = Vector3::VSub(_end, _start);
		float segmentLengthSq = Vector3::SqrMagnitude(segment);
		if (segmentLengthSq < EPSILON)
			return _start; // 線分がほぼ点の場合、開始点を返す

		float t = Vector3::Dot(Vector3::VSub(_point, _start), segment) / segmentLengthSq;
		t = std::clamp(t, 0.0f, 1.0f); // tを0から1の範囲に制限
		return Vector3::VAdd(_start, Vector3::VScale(segment, t));
	}

	/*
	 * @brief 2つの線分の最近点を求める
	 */
	std::pair<Vector3, Vector3> ClosestPointsSegmentSegment(const Vector3& p1, const Vector3& q1, const Vector3& p2, const Vector3& q2) {
		Vector3 d1 = Vector3::Angle(q1, p1); // 線分1の方向ベクトル
		Vector3 d2 = Vector3::Angle(q2, p2); // 線分2の方向ベクトル
		Vector3 r = Vector3::Angle(p1, p2);

		float a = Vector3::Dot(d1, d1); // 線分1の長さの2乗
		float e = Vector3::Dot(d2, d2); // 線分2の長さの2乗
		float f = Vector3::Dot(d2, r);

		float s, t;

		// 線分がほぼ点の場合の処理
		if (a <= EPSILON && e <= EPSILON)
			return { p1, p2 };

		if (a <= EPSILON) {
			// 線分1がほぼ点の場合
			s = 0.0f;
			t = std::clamp(f / e, 0.0f, 1.0f);
		}
		else {
			float c = Vector3::Dot(d1, r);
			if (e <= EPSILON) {
				// 線分2がほぼ点の場合
				t = 0.0f;
				s = std::clamp(-c / a, 0.0f, 1.0f);
			}
			else {
				float b = Vector3::Dot(d1, d2);
				float denom = a * e - b * b;

				// 線分が平行でない場合
				if (denom > EPSILON) {
					s = std::clamp((b * f - c * e) / denom, 0.0f, 1.0f);
				}
				else {
					s = 0.0f; // 線分が平行の場合、sを0に設定
				}

				t = (b * s + f) / e;

				// tを0から1の範囲に制限
				if (t < 0.0f) {
					t = 0.0f;
					s = std::clamp(-c / a, 0.0f, 1.0f);
				}
				else if (t > 1.0f) {
					t = 1.0f;
					s = std::clamp((b - c) / a, 0.0f, 1.0f);
				}
			}
		}

		return { Vector3::VAdd(p1, Vector3::VScale(d1, s)), Vector3::VAdd(p2, Vector3::VScale(d2, t)) };
	}

	/*
	 * @brief pを範囲内に丸める
	 */
	Vector3 ClampToBox(const Vector3& _p, const Vector3& _boxCenter, const Vector3& _boxHalfExtent) {
		return Vector3(
			std::clamp(_p.x, _boxCenter.x - _boxHalfExtent.x, _boxCenter.x + _boxHalfExtent.x),
			std::clamp(_p.y, _boxCenter.y - _boxHalfExtent.y, _boxCenter.y + _boxHalfExtent.y),
			std::clamp(_p.z, _boxCenter.z - _boxHalfExtent.z, _boxCenter.z + _boxHalfExtent.z)
		);
	}

	/*
	 * @brief 線分と箱の最近点を求める
	 */
	std::pair<Vector3, Vector3> ClosestPointsSegmentBox(const Vector3& _segStart, const Vector3& _segEnd, const Vector3& _boxCenter, const Vector3& _boxHalfExtent) {
		constexpr int ITERATIONS = 8;	// 丸める回数

		Vector3 p = _segStart;
		for (int i = 0; i < ITERATIONS; i++) {
			Vector3 q = ClampToBox(p, _boxCenter, _boxHalfExtent);
			p = ClosestPointOnSegment(q, _segStart, _segEnd);
		}
		return { p,ClampToBox(p,_boxCenter,_boxHalfExtent) };
	}

	bool RayVsSphere(const Vector3& _origin, const Vector3& _dir, const Vector3& _center, float _radius, float _dist, float& _hitDist) {
		Vector3 m = Vector3::VSub(_origin, _center);
		float b = Vector3::Dot(m, _dir);
		float c = Vector3::Dot(m, m) - std::pow(_radius, 2);

		// 起点が球の外側にあり、かつ球と逆方向の場合は当たらない
		if (c > 0.0f && b > 0.0f) return false;

		float discr = b * b - c;
		if (discr < 0.0f) return 0.0f;

		// 起点が球の中なら当たったことにする
		float t = -b - std::sqrtf(discr);
		if (t < 0.0f) t = 0.0f;
		if (t > _dist) return false;

		_hitDist = t;
		return true;
	}

	bool RayVsBoxAABB(const Vector3& _origin, const Vector3& _dir, const Vector3& _center, const Vector3& _halfExtent, float _dist, float& _hitDist, Vector3& _hitNormal) {
		float tMin = 0.0f;
		float tMax = _dist;
		Vector3 normal = VZero;

		std::array<float, 3> origin = _origin.GetArray();
		std::array<float, 3> dir = _dir.GetArray();
		std::array<float, 3> center = _center.GetArray();
		std::array<float, 3> halfExtent = _halfExtent.GetArray();
		std::array<Vector3, 3> normalScale = { VRight,VUp,VForward };

		for (int i = 0, max = 3; i < max; i++) {
			if (std::abs(dir[i]) < EPSILON) {
				if (origin[i] < center[i] - halfExtent[i] || origin[i] > center[i] + halfExtent[i])
					return false;
			}
			else {
				float inv = 1.0f / dir[i];
				float t1 = (center[i] - halfExtent[i] - origin[i]) * inv;
				float t2 = (center[i] + halfExtent[i] - origin[i]) * inv;
				float sign = -1.0f;
				if (t1 > t2) {
					std::swap(t1, t2);
					sign = 1.0f;
				}
				if (t1 > tMin) {
					tMin = t1;
					normal = Vector3::VScale(normalScale[i], sign);
				}
				if (t2 < tMax)
					tMax = t2;
				if (tMin > tMax)
					return false;
			}
		}

		_hitDist = tMin;
		_hitNormal = normal;
		return true;
	}

	bool RayVsCapsule(const Vector3& _origin, const Vector3& _dir, const Vector3& _start, const Vector3& _end, float _radius, float _dist, float& _hitDist, Vector3& _hitNormal) {
		Vector3 d = Vector3::VSub(_end, _start);
		Vector3 m = Vector3::VSub(_origin, _start);
		float dd = Vector3::Dot(d, d);

		bool hasHit = false;
		float best = _dist;

		// 円柱側面
		if (dd > EPSILON) {
			float nd = Vector3::Dot(_dir, d);
			float mn = Vector3::Dot(m, _dir);
			float mm = Vector3::Dot(m, m);
			float md = Vector3::Dot(m, d);

			float a = dd - nd * nd;
			float b = dd * mn - nd * md;
			float c = dd * (mm - _radius * _radius) - md * md;

			if (std::abs(a) > EPSILON) {
				float discr = b * b - a * c;
				if (discr >= 0.0f) {
					float sqrtDiscr = std::sqrt(discr);
					float t = (-b - sqrtDiscr) / a;
					// 起点が円柱内部にある場合は出口を見る
					if (t < 0.0f) t = (-b + sqrtDiscr);
					if (t >= 0.0f && t <= best) {
						// 軸方向のパラメータ
						float k = md + t * nd;
						if (k >= 0.0f && k <= dd) {
							hasHit = true;
							best = t;
						}
					}
				}
			}
		}

		// 両端の半球
		float cap = 0.0f;
		if (RayVsSphere(_origin, _dir, _start, _radius, best, cap)) {
			hasHit = true;
			best = cap;
		}
		if (RayVsSphere(_origin, _dir, _end, _radius, best, cap)) {
			hasHit = true;
			best = cap;
		}

		if (!hasHit) return false;

		_hitDist = best;
		Vector3 hitPoint = Vector3::VAdd(_origin, Vector3::VScale(_dir, best));
		Vector3 closestOnAxis = ClosestPointOnSegment(hitPoint, _start, _end);
		Vector3 n = Vector3::VSub(hitPoint, closestOnAxis);
		float len = n.Magnitude();
		_hitNormal = (len > EPSILON) ? Vector3(n.x / len, n.y / len, n.z / len) : VRight;

		return true;
	}
}

void NarrowPhaseInitialize() {
	// 衝突判定関数の初期化
	table[ColliderType::Sphere][ColliderType::Sphere] = TestSphereSphere;
	table[ColliderType::Sphere][ColliderType::Capsule] = TestSphereCapsule;
	table[ColliderType::Sphere][ColliderType::BoxAABB] = TestSphereAABB;
	table[ColliderType::Capsule][ColliderType::Capsule] = TestCapsuleCapsule;
	table[ColliderType::Capsule][ColliderType::BoxAABB] = TestCapsuleAABB;
	table[ColliderType::BoxAABB][ColliderType::BoxAABB] = TestAABBAABB;
}

NarrowPhaseFunc GetNarrowPhaseFunc(ColliderType _typeA, ColliderType _typeB) {
	return table[_typeA][_typeB];
}

bool TestCollision(const ColliderData& _a, const ColliderData& _b, HitResult& _result) {
	NarrowPhaseFunc func = GetNarrowPhaseFunc(_a.shape.type, _b.shape.type);
	if (!func) return false;
	return func(_a, _b, _result);
}

bool TestSphereSphere(const ColliderData& _a, const ColliderData& _b, HitResult& _result) {
	const SphereShape& sphereA = _a.shape.sphere;
	const SphereShape& sphereB = _b.shape.sphere;

	float dx = _b.worldPos.x - _a.worldPos.x;
	float dy = _b.worldPos.y - _a.worldPos.y;
	float dz = _b.worldPos.z - _a.worldPos.z;
	float distSq = Vector3::SqrMagnitude(dx, dy, dz);
	float radiusSum = sphereA.radius + sphereB.radius;

	// 衝突していなければfalseを返す
	if (distSq > powf(radiusSum, 2)) return false;

	float dist = sqrtf(distSq);

	// 衝突点の計算
	// 完全一致は法線を適当に設定する
	Vector3 normal = (dist > EPSILON) ? Vector3(dx / dist, dy / dist, dz / dist) : VOne;

	_result.normal = normal;
	_result.penetration = radiusSum - dist;
	_result.point = Vector3::VAdd(_a.worldPos, Vector3::VScale(normal, sphereA.radius));

	return true;
}

bool TestSphereCapsule(const ColliderData& _sphere, const ColliderData& _capsule, HitResult& _result) {
	const SphereShape& sphere = _sphere.shape.sphere;
	const CapsuleShape& capsule = _capsule.shape.capsule;

	Vector3 capsuleStart = _capsule.worldPos;
	Vector3 capsuleEnd = Vector3::VAdd(_capsule.worldPos, capsule.localOffset);

	Vector3 closest = ClosestPointOnSegment(_sphere.worldPos, capsuleStart, capsuleEnd);

	// 最近点と球の中心の距離を計算
	Vector3 diff = Vector3::VSub(closest, _sphere.worldPos);
	float distSq = Vector3::SqrMagnitude(diff);

	float radiusSum = sphere.radius + capsule.radius;
	if (distSq > pow(radiusSum, 2)) return false;

	float dist = sqrtf(distSq);
	Vector3 normal = (dist > EPSILON) ? Vector3(diff.x / dist, diff.y / dist, diff.z / dist) : VRight;

	_result.normal = normal;
	_result.penetration = radiusSum - dist;
	_result.point = Vector3::VAdd(_sphere.worldPos, Vector3::VScale(normal, sphere.radius));

	return true;
}

bool TestSphereAABB(const ColliderData& _sphere, const ColliderData& _aabb, HitResult& _result) {
	const SphereShape& sphere = _sphere.shape.sphere;
	const AABBShape& box = _aabb.shape.aabb;

	// AABBの最も近い点を求める
	float closestX = std::clamp(_sphere.worldPos.x, _aabb.worldPos.x - box.halfExtent.x, _aabb.worldPos.x + box.halfExtent.x);
	float closestY = std::clamp(_sphere.worldPos.y, _aabb.worldPos.y - box.halfExtent.y, _aabb.worldPos.y + box.halfExtent.y);
	float closestZ = std::clamp(_sphere.worldPos.z, _aabb.worldPos.z - box.halfExtent.z, _aabb.worldPos.z + box.halfExtent.z);

	float dx = _sphere.worldPos.x - closestX;
	float dy = _sphere.worldPos.y - closestY;
	float dz = _sphere.worldPos.z - closestZ;
	float distSq = Vector3::SqrMagnitude(dx, dy, dz);

	// 衝突していなければfalseを返す
	if (distSq > powf(sphere.radius, 2)) return false;

	float dist = sqrtf(distSq);

	if (dist > EPSILON) {
		_result.normal = Vector3(dx / dist, dy / dist, dz / dist);
		_result.penetration = sphere.radius - dist;
		_result.point = Vector3(closestX, closestY, closestZ);
		return true;
	}

	// めり込みケース
	float px = box.halfExtent.x - std::abs(_sphere.worldPos.x - _aabb.worldPos.x);
	float py = box.halfExtent.y - std::abs(_sphere.worldPos.y - _aabb.worldPos.y);
	float pz = box.halfExtent.z - std::abs(_sphere.worldPos.z - _aabb.worldPos.z);

	if (px <= py && px <= pz) {
		_result.normal = _sphere.worldPos.x >= _aabb.worldPos.x ? VRight : VLeft;
		_result.penetration = px + sphere.radius;
	}
	else if (py <= px && py <= pz) {
		_result.normal = _sphere.worldPos.y >= _aabb.worldPos.y ? VUp : VDown;
		_result.penetration = py + sphere.radius;
	}
	else {
		_result.normal = _sphere.worldPos.z >= _aabb.worldPos.z ? VForward : VBack;
		_result.penetration = pz + sphere.radius;
	}
	_result.point = _sphere.worldPos;
	return true;
}

bool TestCapsuleCapsule(const ColliderData& _a, const ColliderData& _b, HitResult& _result) {
	const CapsuleShape& capA = _a.shape.capsule;
	const CapsuleShape& capB = _b.shape.capsule;

	Vector3 aStart = _a.worldPos;
	Vector3 aEnd = Vector3::VAdd(aStart, capA.localOffset);
	Vector3 bStart = _b.worldPos;
	Vector3 bEnd = Vector3::VAdd(bStart, capB.localOffset);

	// 線分同士の最近点
	Vector3 closestA, closestB;
	std::pair<Vector3, Vector3> closest = ClosestPointsSegmentSegment(aStart, aEnd, bStart, bEnd);

	closestA = closest.first;
	closestB = closest.second;

	Vector3 diff = Vector3::VSub(closestB, closestA);
	float distSq = Vector3::SqrMagnitude(diff);

	float radiusSum = capA.radius + capB.radius;
	if (distSq > pow(radiusSum, 2)) return false;

	float dist = sqrtf(distSq);
	Vector3 normal = (dist > EPSILON) ? Vector3(diff.x / dist, diff.y / dist, diff.z / dist) : VRight;

	_result.normal = normal;
	_result.penetration = radiusSum - dist;
	_result.point = Vector3::VAdd(closestA, Vector3::VScale(normal, capA.radius));

	return true;
}

bool TestCapsuleAABB(const ColliderData& _capsule, const ColliderData& _aabb, HitResult& _result) {
	const CapsuleShape& capsule = _capsule.shape.capsule;
	const AABBShape& box = _aabb.shape.aabb;

	Vector3 capStart = _capsule.worldPos;
	Vector3 capEnd = capsule.localOffset;
	const Vector3& boxCenter = _aabb.worldPos;
	const Vector3& boxHalf = box.halfExtent;

	// カプセルの線分とAABBの最近点
	Vector3 onSegment, onBox;
	std::pair<Vector3, Vector3> pair = ClosestPointsSegmentBox(capStart, capEnd, boxCenter, boxHalf);

	onSegment = pair.first;
	onBox = pair.second;

	Vector3 diff = Vector3::VSub(onBox, onSegment);
	float distSq = Vector3::SqrMagnitude(diff);

	// 衝突していなければfalseを返す
	if (distSq > powf(capsule.radius, 2)) return false;

	if (distSq > EPSILON) {
		// 線分がAABBの外にある場合
		if (distSq > pow(capsule.radius, 2))return false;

		float dist = sqrtf(distSq);
		_result.normal = Vector3(diff.x / dist, diff.y / dist, diff.z / dist);
		_result.penetration = capsule.radius - dist;
		_result.point = onBox;
		return true;
	}

	// めり込みケース
	float px = boxHalf.x - std::abs(onSegment.x - boxCenter.x);
	float py = boxHalf.y - std::abs(onSegment.y - boxCenter.y);
	float pz = boxHalf.z - std::abs(onSegment.z - boxCenter.z);

	if (px <= py && px <= pz) {
		_result.normal = onSegment.x >= boxCenter.x ? VRight : VLeft;
		_result.penetration = px + capsule.radius;
	}
	else if (py <= px && py <= pz) {
		_result.normal = onSegment.y >= boxCenter.y ? VUp : VDown;
		_result.penetration = py + capsule.radius;
	}
	else {
		_result.normal = onSegment.z >= boxCenter.z ? VForward : VBack;
		_result.penetration = pz + capsule.radius;
	}
	_result.point = onSegment;
	return true;
}

bool TestAABBAABB(const ColliderData& _a, const ColliderData& _b, HitResult& _result) {
	const AABBShape& boxA = _a.shape.aabb;
	const AABBShape& boxB = _b.shape.aabb;

	Vector3 dir = Vector3::Angle(_b.worldPos, _a.worldPos);
	Vector3 absDist = Vector3(std::_Float_abs(dir.x), std::_Float_abs(dir.y), std::_Float_abs(dir.z));
	// めり込み量の計算
	Vector3 overlap = Vector3::VSub(Vector3::VAdd(boxA.halfExtent, boxB.halfExtent), absDist);
	// 交差していなければfalseを返す
	if (overlap.x <= 0.0f || overlap.y <= 0.0f || overlap.z <= 0.0f) return false;

	// 最小のめり込み量を持つ軸を分離軸として使う
	if (overlap.x < overlap.y && overlap.x < overlap.z) {
		_result.normal = dir.x >= 0.0f ? VRight : VLeft;
		_result.penetration = overlap.x;
	}
	else if (overlap.y <= overlap.x && overlap.y <= overlap.z) {
		_result.normal = dir.y >= 0.0f ? VUp : VDown;
		_result.penetration = overlap.y;
	}
	else {
		_result.normal = dir.z >= 0.0f ? VForward : VBack;
		_result.penetration = overlap.z;
	}

	// 最近点は中心にする
	_result.point = Vector3::VScale(Vector3::VAdd(_a.worldPos, _b.worldPos), 0.5f);

	return true;
}

bool RayVsCollider(const Vector3& _origin, const Vector3& _dir, const ColliderData _collider, float _dist, float& _hitDist, Vector3& _hitNormal) {
	switch (_collider.shape.type) {
	case ColliderType::Sphere:
	{
		const SphereShape& sphere = _collider.shape.sphere;

		if (!RayVsSphere(_origin, _dir, _collider.worldPos, sphere.radius, _dist, _hitDist)) return false;

		Vector3 hitPoint = Vector3::VAdd(_origin, Vector3::VScale(_dir, _hitDist));
		Vector3 n = Vector3::VSub(hitPoint, _collider.worldPos);
		float len = n.Magnitude();
		_hitNormal = (len > EPSILON) ? Vector3(n.x / len, n.y / len, n.z / len) : VRight;
	}

	return true;
	case ColliderType::BoxAABB:
	{

		const AABBShape& aabb = _collider.shape.aabb;
		return RayVsBoxAABB(_origin, _dir, _collider.worldPos, aabb.halfExtent, _dist, _hitDist, _hitNormal);
	}
	case ColliderType::Capsule:
	{

		const CapsuleShape& cap = _collider.shape.capsule;
		Vector3 end = Vector3::VAdd(_collider.worldPos, cap.localOffset);
		return RayVsCapsule(_origin, _dir, _collider.worldPos, end, cap.radius, _dist, _hitDist, _hitNormal);
	}
	}
}
