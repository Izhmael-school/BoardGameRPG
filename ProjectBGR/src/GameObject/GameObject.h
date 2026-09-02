/*
 * @brief すべてのオブジェクトの基底
 * @author Sekino
 */
#pragma once
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Definition/Enum/Tag.h"
#include <vector>
#include <memory>
#include <string>
#include "Library/Vector/Vector3.h"
#include "Component/Transform/Transform.h"
#include <typeindex>

class ComponentBase;
class ColliderData;

class GameObject {
protected:
	int modelHandle;	// モデル番号

	bool isActive;		// 有効か

	bool wantDelete;	// 管理している存在に削除を要請する

	Tag tag;			// タグ

	Transform* pTransform;

	std::string name;

	std::type_index classID;

	std::vector<std::unique_ptr<ComponentBase>> components;
public:
	GameObject(int _modelHandle = -1, const std::string& _name = "GameObject", Vector3 _pos = VZero, Vector3 _rot = VZero, Vector3 _scale = VOne, Tag _tag = Notag);
	virtual ~GameObject();

protected:
	/*
	 * @brief 生成時処理
	 */
	virtual void Start();
public:
	/*
	 * @brief 更新
	 */
	virtual void Update(float _t);

	/*
	 * @brief 描画
	 */
	virtual void Render();

	/*
	 * @brief 初期化
	 */
	virtual void Setup();

	/*
	 * @brief 使用後処理
	 */
	virtual void Cleanup();
protected:
	/*
	 * @brief 有効化時処理
	 */
	virtual void Enable();

	/*
	 * @brief 無効化時処理
	 */
	virtual void Disable();

protected:
	/*
	 * @brief classIDのセッター
	 * @brief プーリングするときにクラスのハッシュを使って作るために継承先で宣言必要があるため。
	 */
	virtual void SetClassID();


public:	// 衝突判定　基本はColliderコンポーネントがついてるときだけ関数に入る
	/*
	 * @brief 当たった
	 */
	virtual void OnTriggerEnter(ColliderData& _pSelf, ColliderData& _pOther);

	/*
	 * @brief 当たった
	 */
	virtual void OnTriggerStay(ColliderData& _pSelf, ColliderData& _pOther);

	/*
	 * @brief 当たった
	 */
	virtual void OnTriggerExit(ColliderData& _pSelf, ColliderData& _pOther);

	/*
	 * @brief 当たった
	 */
	virtual void OnCollisionEnter(ColliderData& _pSelf, ColliderData& _pOther);

	/*
	 * @brief 当たった
	 */
	virtual void OnCollisionStay(ColliderData& _pSelf, ColliderData& _pOther);

	/*
	 * @brief 当たった
	 */
	virtual void OnCollisionExit(ColliderData& _pSelf, ColliderData& _pOther);

public:
	/*
	 * @brief classIDのゲッター
	 */
	std::type_index GetClassID() const { return classID; }
public:
	/*
	 * @brief modelHandleのセッター
	 */
	inline void SetModelHandle(int _modelHandle) { modelHandle = _modelHandle; }

	/*
	 * @brief modelHandleのゲッター
	 */
	inline int GetModelHandle() const { return modelHandle; }

	/*
	 * @brief isActiveのセッター
	 */
	void SetActive(bool _isActive);

	/*
	 * @brief isActiveのゲッター
	 */
	inline bool IsActive() const { return isActive; }

	/*
	 * @brief wantDeleteのセッター
	 */
	inline void SetWantDelete(bool _wantDelete) { wantDelete = _wantDelete; }

	/*
	 * @brief wantDeleteのゲッター
	 */
	inline bool WantDelete() const { return wantDelete; }

	/*
	 * @brief pTransformのゲッター
	 */
	inline Transform* GetTransform() const { return pTransform; }

	/*
	 * @brief tagのセッター
	 */
	inline void SetTag(Tag _tag) { tag = _tag; }

	/*
	 * @brief tagのゲッター
	 */
	inline Tag GetTag() const { return tag; }

	/*
	 * @brief 引数のタグが自身と同じか
	 * @return 引数が自身のタグと一致してるか
	 */
	bool CompareTag(Tag _tag) const;

	/*
	 * @brief コンポーネントの追加
	 */
	template<typename T, typename... Args>
	T* AddComponent(Args&&... args);

	/*
	 * @brief コンポーネントの取得
	 */
	template<typename T>
	T* GetComponent();
};

template<typename T, typename ...Args>
inline T* GameObject::AddComponent(Args&&... args) {
	// コンポーネントで無ければ帰る
	if (!std::is_base_of<ComponentBase, T>::value) return nullptr;
	// 生成
	std::unique_ptr<T> component = std::make_unique<T>(this, std::forward<Args>(args)...);
	components.push_back(std::move(component));
	return dynamic_cast<T*>(components.back().get());
}

template<typename T>
inline T* GameObject::GetComponent() {

	for (auto& c : components) {
		T* component = dynamic_cast<T*>(c.get());
		if (!component) continue;

		return component;
	}
	return nullptr;
}

#endif // !_GAMEOBJECT_H_