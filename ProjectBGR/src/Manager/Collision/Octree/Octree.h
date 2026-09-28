#pragma once
#include "Vector3.h"
#include <vector>
#include <memory>

/*
 * @brief AABB縺ｮ繝・・繧ｿ
 */
struct AABB {
	Vector3 center;		// 荳ｭ蠢・ｺｧ讓・
	Vector3 halfSize;	// 蜷・ｻｸ縺ｮ蜊雁・縺ｮ髟ｷ縺・

	/*
	 * @brief AABB縺ｮ莠､蟾ｮ蛻､螳・
	 */
	bool Intersects(const AABB& other) const;
	/*
	 * @brief AABB縺ｫ蜷ｫ縺ｾ繧後ｋ縺九・蛻､螳・
	 */
	bool Contains(const AABB& other) const;
};

/*
 * @brief Octree縺ｫ譬ｼ邏阪☆繧九が繝悶ず繧ｧ繧ｯ繝医・繝・・繧ｿ
 */
struct OctreeObject {
	AABB bounds;	// 繧ｪ繝悶ず繧ｧ繧ｯ繝医・AABB
	int handle;	// 繧ｪ繝悶ず繧ｧ繧ｯ繝医∈縺ｮ繝昴う繝ｳ繧ｿ
};

class OctreeNode {
public:
	AABB bounds;	// 繝弱・繝峨・AABB
	std::vector<OctreeObject> objects;	// 繝弱・繝峨↓譬ｼ邏阪＆繧後※縺・ｋ繧ｪ繝悶ず繧ｧ繧ｯ繝・
	std::unique_ptr<OctreeNode> children[8];	// 蟄舌ヮ繝ｼ繝・
	int depth;	// 繝弱・繝峨・豺ｱ縺・

	static constexpr int MAX_OBJECTS = 8;	// 繝弱・繝峨↓譬ｼ邏阪〒縺阪ｋ譛螟ｧ繧ｪ繝悶ず繧ｧ繧ｯ繝域焚
	static constexpr int MAX_DEPTH = 6;	// 繝弱・繝峨・譛螟ｧ豺ｱ縺・

public:
	OctreeNode(const AABB& _bounds, int _depth);

	/*
	 * @brief 蟄舌ヮ繝ｼ繝峨°
	 */
	inline bool IsLeaf() const { return children[0] == nullptr; }

	/*
	 * @brief 繝弱・繝峨ｒ蛻・牡
	 */
	void Subdivide();

	/*
	 * @brief 繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ謖ｿ蜈･
	 */
	void Insert(const OctreeObject& _obj);

	/*
	 * @brief 遽・峇蜀・・繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ辣ｧ莨・
	 */
	void Query(const AABB& _range, std::vector<OctreeObject>& _result);

	/*
	 * @brief 繝弱・繝峨ｒ繧ｯ繝ｪ繧｢
	 */
	void Clear();
};

class CollisionManager;

class OctreeWorld {
private:
	std::unique_ptr<OctreeNode> root;	// 繝ｫ繝ｼ繝医ヮ繝ｼ繝・
	AABB worldBounds;	// 繝ｯ繝ｼ繝ｫ繝峨・AABB

public:
	OctreeWorld(const AABB& _bounds);

public:
	/*
	 * @brief Octree繧貞・讒狗ｯ・
	 */
	void Rebuild(CollisionManager& _pManager);

	/*
	 * @brief 遽・峇蜀・・繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ辣ｧ莨・
	 */
	std::vector<OctreeObject> Query(const AABB& _range);

	/*
	 * @brief 縺吶∋縺ｦ縺ｮ陦晉ｪ√・繧｢繧貞庶髮・
	 */
	void CollectAllPairs(std::vector<std::pair<int, int>>& _outPairs) const;

private:
	/* 
	 * @brief 蜀榊ｸｰ逧・↓陦晉ｪ√・繧｢繧貞庶髮・
	 */
	void CollectPairsRecursive(const OctreeNode* _node,const std::vector<OctreeObject> ancestorObjects,std::vector<std::pair<int, int>>& _outPairs) const;
};