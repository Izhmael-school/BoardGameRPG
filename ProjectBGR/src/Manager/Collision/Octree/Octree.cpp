#include "Octree.h"
#include "../CollisionManager.h"

OctreeNode::OctreeNode(const AABB& _bounds, int _depth)
	: bounds(_bounds), depth(_depth) {
}

void OctreeNode::Subdivide() {
	Vector3 quarter = Vector3::VScale(bounds.halfSize, 0.5f);
	// 蟄舌ヮ繝ｼ繝峨ｒ逕滓・
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
	// 繧ｪ繝悶ず繧ｧ繧ｯ繝医′繝弱・繝峨・遽・峇螟悶↑繧牙ｸｰ繧・
	if (!bounds.Intersects(_obj.bounds)) return;

	// 蟄舌ヮ繝ｼ繝峨′縺ゅｋ縺・
	if (IsLeaf()) {
		objects.push_back(_obj);
		// 荳企剞縺ｪ繧牙・蜑ｲ縺励※蜀埼・鄂ｮ
		if (objects.size() > MAX_OBJECTS && depth < MAX_DEPTH) {
			Subdivide();
			// 譌｢蟄倥・繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ蜀埼・鄂ｮ
			for (const auto& obj : objects) {
				for (const auto& child : children) {
					child->Insert(obj);
				}
			}
			objects.clear();
		}
		return;
	}

	// 蟄舌ヮ繝ｼ繝峨↓繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ謖ｿ蜈･
	for (const auto& child : children) {
		child->Insert(_obj);
	}
}

void OctreeNode::Query(const AABB& _range, std::vector<OctreeObject>& _result) {
	// 莠､蟾ｮ縺励※縺ｪ縺代ｌ縺ｰ蟶ｰ繧・
	if (!bounds.Intersects(_range)) return;
	// 莠､蟾ｮ縺励※縺・ｋ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ霑ｽ蜉
	for (const auto& obj : objects) {
		if (_range.Intersects(obj.bounds)) {
			_result.push_back(obj);
		}
	}
	// 蟄舌ヮ繝ｼ繝峨′縺ｪ縺代ｌ縺ｰ蟶ｰ繧・
	if (IsLeaf()) return;
	// 蟄舌ヮ繝ｼ繝峨↓蝠上＞蜷医ｏ縺・
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
	// 譛牙柑縺ｪ繧ｳ繝ｩ繧､繝繝ｼ繧丹ctree縺ｫ謖ｿ蜈･
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

	// 蜷御ｸ繝弱・繝牙・縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝亥酔螢ｫ縺ｮ陦晉ｪ√・繧｢繧貞庶髮・
	for (int i = 0, max = _node->objects.size(); i < max; ++i) {
		for (int j = i + 1; j < max; ++j) {
			const auto& a = _node->objects[i];
			const auto& b = _node->objects[j];
			// 莠､蟾ｮ縺励※縺・↑縺代ｌ縺ｰ谺｡
			if (!a.bounds.Intersects(b.bounds))continue;
			_outPairs.emplace_back(a.handle, b.handle);
		}
	}

	// 逾門・繝弱・繝峨・繧ｪ繝悶ず繧ｧ繧ｯ繝医→縺ｮ陦晉ｪ√・繧｢繧貞庶髮・
	for(const auto& obj : _node->objects) {
		for(const auto& ancestorObj : ancestorObjects) {
			if (!obj.bounds.Intersects(ancestorObj.bounds)) continue;
			_outPairs.emplace_back(obj.handle, ancestorObj.handle);
		}
	}

	if (_node->IsLeaf()) return;
	// 逾門・繝ｪ繧ｹ繝医ｒ菴懊ｋ
	std::vector<OctreeObject> nextAncestorObjects = ancestorObjects;
	nextAncestorObjects.insert(nextAncestorObjects.end(), _node->objects.begin(), _node->objects.end());
	// 蟄舌ヮ繝ｼ繝峨↓蟇ｾ縺励※蜀榊ｸｰ逧・↓蜻ｼ縺ｳ蜃ｺ縺・
	for (const auto& child : _node->children) {
		CollectPairsRecursive(child.get(), nextAncestorObjects, _outPairs);
	}
}
