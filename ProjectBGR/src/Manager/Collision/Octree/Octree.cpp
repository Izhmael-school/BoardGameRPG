#include "Octree.h"
#include "../CollisionManager.h"

OctreeNode::OctreeNode(const AABB& _bounds, int _depth)
	: bounds(_bounds), depth(_depth) {
}

void OctreeNode::Subdivide() {
	Vector3 quarter = Vector3::VScale(bounds.halfSize, 0.5f);
	// 子ノードを生成
	for (int i = 0; i < MAX_OBJECTS; ++i) {
		Vector3 offset = Vector3(
			(i & 0b0001 ? quarter.x : -quarter.x),
			(i & 0b0010 ? quarter.y : -quarter.y),
			(i & 0b0100 ? quarter.z : -quarter.z)
		);
		AABB childBounds = { Vector3::VAdd(bounds.center, offset), quarter };
		children[i] = std::make_unique<OctreeNode>(childBounds, depth + 1);
	}
}

void OctreeNode::Insert(const OctreeObject& _obj) {
	// オブジェクトがノードの範囲外なら帰る
	if (!bounds.Intersects(_obj.bounds)) return;

	// 子ノードがあるか
	if (IsLeaf()) {
		objects.push_back(_obj);
		// 上限なら分割して再配置
		if (objects.size() > MAX_OBJECTS && depth < MAX_DEPTH) {
			Subdivide();
			// 既存のオブジェクトを再配置
			for (const auto& obj : objects) {
				for (const auto& child : children) {
					child->Insert(obj);
				}
			}
			objects.clear();
		}
		return;
	}

	// 子ノードにオブジェクトを挿入
	for (const auto& child : children) {
		child->Insert(_obj);
	}
}

void OctreeNode::Query(const AABB& _range, std::vector<OctreeObject>& _result) {
	// 交差してなければ帰る
	if (!bounds.Intersects(_range)) return;
	// 交差しているオブジェクトを追加
	for (const auto& obj : objects) {
		if (_range.Intersects(obj.bounds)) {
			_result.push_back(obj);
		}
	}
	// 子ノードがなければ帰る
	if (IsLeaf()) return;
	// 子ノードに問い合わせ
	for (const auto& child : children) {
		child->Query(_range, _result);
	}
}

void OctreeNode::Clear() {
	objects.clear();
	for (auto& child : children) {
		child.reset();
	}
}

bool AABB::Intersects(const AABB& other) const {
	return std::abs(center.x - other.center.x) <= (halfSize.x + other.halfSize.x) &&
		std::abs(center.y - other.center.y) <= (halfSize.y + other.halfSize.y) &&
		std::abs(center.z - other.center.z) <= (halfSize.z + other.halfSize.z);
}

bool AABB::Contains(const AABB& other) const {
	return (other.center.x - other.halfSize.x >= center.x - halfSize.x) &&
		(other.center.x + other.halfSize.x <= center.x + halfSize.x) &&
		(other.center.y - other.halfSize.y >= center.y - halfSize.y) &&
		(other.center.y + other.halfSize.y <= center.y + halfSize.y) &&
		(other.center.z - other.halfSize.z >= center.z - halfSize.z) &&
		(other.center.z + other.halfSize.z <= center.z + halfSize.z);
}

OctreeWorld::OctreeWorld(const AABB& _bounds) 
	:worldBounds(_bounds)
{
	root = std::make_unique<OctreeNode>(_bounds, 0);
}

void OctreeWorld::Rebuild(CollisionManager& _pManager) {
	root->Clear();

	const auto& all = _pManager.GetColliders();
	// 有効なコライダーをOctreeに挿入
	for (int i = 0, max = all.size(); i < max; ++i) {
		const ColliderData& c = all[i];
		if (!c.isEnable) continue;

		int handle = _pManager.GetHandleAt(i);
		OctreeObject obj = { c.worldAABB, handle };
		root->Insert(obj);
	}
}

std::vector<OctreeObject> OctreeWorld::Query(const AABB& _range) {
	std::vector<OctreeObject> results;
	root->Query(_range, results);
	return results;
}

void OctreeWorld::CollectAllPairs(std::vector<std::pair<int, int>>& _outPairs) const {
	std::vector<OctreeObject> ancestorObjects;
	CollectPairsRecursive(root.get(), ancestorObjects, _outPairs);
}

void OctreeWorld::CollectPairsRecursive(const OctreeNode* _node, const std::vector<OctreeObject> ancestorObjects, std::vector<std::pair<int, int>>& _outPairs) const {
	if(!_node) return;

	// 同一ノード内のオブジェクト同士の衝突ペアを収集
	for (int i = 0, max = _node->objects.size(); i < max; ++i) {
		for (int j = i + 1; j < max; ++j) {
			const auto& a = _node->objects[i];
			const auto& b = _node->objects[j];
			// 交差していなければ次
			if (!a.bounds.Intersects(b.bounds))continue;
			_outPairs.emplace_back(a.handle, b.handle);
		}
	}

	// 祖先ノードのオブジェクトとの衝突ペアを収集
	for(const auto& obj : _node->objects) {
		for(const auto& ancestorObj : ancestorObjects) {
			if (!obj.bounds.Intersects(ancestorObj.bounds)) continue;
			_outPairs.emplace_back(obj.handle, ancestorObj.handle);
		}
	}

	if (_node->IsLeaf()) return;
	// 祖先リストを作る
	std::vector<OctreeObject> nextAncestorObjects = ancestorObjects;
	nextAncestorObjects.insert(nextAncestorObjects.end(), _node->objects.begin(), _node->objects.end());
	// 子ノードに対して再帰的に呼び出す
	for (const auto& child : _node->children) {
		CollectPairsRecursive(child.get(), nextAncestorObjects, _outPairs);
	}
}
