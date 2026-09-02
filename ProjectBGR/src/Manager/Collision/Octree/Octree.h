#pragma once
#include "Vector/Vector3.h"
#include <vector>
#include <memory>

/*
 * @brief AABBのデータ
 */
struct AABB {
	Vector3 center;		// 中心座標
	Vector3 halfSize;	// 各軸の半分の長さ

	/*
	 * @brief AABBの交差判定
	 */
	bool Intersects(const AABB& other) const;
	/*
	 * @brief AABBに含まれるかの判定
	 */
	bool Contains(const AABB& other) const;
};

/*
 * @brief Octreeに格納するオブジェクトのデータ
 */
struct OctreeObject {
	AABB bounds;	// オブジェクトのAABB
	int handle;	// オブジェクトへのポインタ
};

class OctreeNode {
public:
	AABB bounds;	// ノードのAABB
	std::vector<OctreeObject> objects;	// ノードに格納されているオブジェクト
	std::unique_ptr<OctreeNode> children[8];	// 子ノード
	int depth;	// ノードの深さ

	static constexpr int MAX_OBJECTS = 8;	// ノードに格納できる最大オブジェクト数
	static constexpr int MAX_DEPTH = 6;	// ノードの最大深さ

public:
	OctreeNode(const AABB& _bounds, int _depth);

	/*
	 * @brief 子ノードか
	 */
	inline bool IsLeaf() const { return children[0] == nullptr; }

	/*
	 * @brief ノードを分割
	 */
	void Subdivide();

	/*
	 * @brief オブジェクトを挿入
	 */
	void Insert(const OctreeObject& _obj);

	/*
	 * @brief 範囲内のオブジェクトを照会
	 */
	void Query(const AABB& _range, std::vector<OctreeObject>& _result);

	/*
	 * @brief ノードをクリア
	 */
	void Clear();
};

class CollisionManager;

class OctreeWorld {
private:
	std::unique_ptr<OctreeNode> root;	// ルートノード
	AABB worldBounds;	// ワールドのAABB

public:
	OctreeWorld(const AABB& _bounds);

public:
	/*
	 * @brief Octreeを再構築
	 */
	void Rebuild(CollisionManager& _pManager);

	/*
	 * @brief 範囲内のオブジェクトを照会
	 */
	std::vector<OctreeObject> Query(const AABB& _range);

	/*
	 * @brief すべての衝突ペアを収集
	 */
	void CollectAllPairs(std::vector<std::pair<int, int>>& _outPairs) const;

private:
	/* 
	 * @brief 再帰的に衝突ペアを収集
	 */
	void CollectPairsRecursive(const OctreeNode* _node,const std::vector<OctreeObject> ancestorObjects,std::vector<std::pair<int, int>>& _outPairs) const;
};