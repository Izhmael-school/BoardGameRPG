/*
 * @brief 縺吶∋縺ｦ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医・蝓ｺ蠎・
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
	int modelHandle;	// 繝｢繝・Ν逡ｪ蜿ｷ

	bool isActive;		// 譛牙柑縺・

	bool wantDelete;	// 邂｡逅・＠縺ｦ縺・ｋ蟄伜惠縺ｫ蜑企勁繧定ｦ∬ｫ九☆繧・

	Tag tag;			// 繧ｿ繧ｰ

	Transform* pTransform;

	std::string name;

	std::type_index classID;

	std::vector<std::unique_ptr<ComponentBase>> components;
public:
	GameObject(int _modelHandle = -1, const std::string& _name = "GameObject", Vector3 _pos = VZero, Vector3 _rot = VZero, Vector3 _scale = VOne, Tag _tag = Notag);
	virtual ~GameObject();

protected:
	/*
	 * @brief 逕滓・譎ょ・逅・
	 */
	virtual void Start();
public:
	/*
	 * @brief 譖ｴ譁ｰ
	 */
	virtual void Update(float _t);

	/*
	 * @brief 謠冗判
	 */
	virtual void Render();

	/*
	 * @brief 蛻晄悄蛹・
	 */
	virtual void Setup();

	/*
	 * @brief 菴ｿ逕ｨ蠕悟・逅・
	 */
	virtual void Cleanup();
protected:
	/*
	 * @brief 譛牙柑蛹匁凾蜃ｦ逅・
	 */
	virtual void Enable();

	/*
	 * @brief 辟｡蜉ｹ蛹匁凾蜃ｦ逅・
	 */
	virtual void Disable();

protected:
	/*
	 * @brief classID縺ｮ繧ｻ繝・ち繝ｼ
	 * @brief 繝励・繝ｪ繝ｳ繧ｰ縺吶ｋ縺ｨ縺阪↓繧ｯ繝ｩ繧ｹ縺ｮ繝上ャ繧ｷ繝･繧剃ｽｿ縺｣縺ｦ菴懊ｋ縺溘ａ縺ｫ邯呎価蜈医〒螳｣險蠢・ｦ√′縺ゅｋ縺溘ａ縲・
	 */
	virtual void SetClassID();


public:	// 陦晉ｪ∝愛螳壹蝓ｺ譛ｬ縺ｯCollider繧ｳ繝ｳ繝昴・繝阪Φ繝医′縺､縺・※繧九→縺阪□縺鷹未謨ｰ縺ｫ蜈･繧・
	/*
	 * @brief 蠖薙◆縺｣縺・
	 */
	virtual void OnTriggerEnter(ColliderData& _pSelf, ColliderData& _pOther);

	/*
	 * @brief 蠖薙◆縺｣縺・
	 */
	virtual void OnTriggerStay(ColliderData& _pSelf, ColliderData& _pOther);

	/*
	 * @brief 蠖薙◆縺｣縺・
	 */
	virtual void OnTriggerExit(ColliderData& _pSelf, ColliderData& _pOther);

	/*
	 * @brief 蠖薙◆縺｣縺・
	 */
	virtual void OnCollisionEnter(ColliderData& _pSelf, ColliderData& _pOther);

	/*
	 * @brief 蠖薙◆縺｣縺・
	 */
	virtual void OnCollisionStay(ColliderData& _pSelf, ColliderData& _pOther);

	/*
	 * @brief 蠖薙◆縺｣縺・
	 */
	virtual void OnCollisionExit(ColliderData& _pSelf, ColliderData& _pOther);

public:
	/*
	 * @brief classID縺ｮ繧ｲ繝・ち繝ｼ
	 */
	std::type_index GetClassID() const { return classID; }
public:
	/*
	 * @brief modelHandle縺ｮ繧ｻ繝・ち繝ｼ
	 */
	inline void SetModelHandle(int _modelHandle) { modelHandle = _modelHandle; }

	/*
	 * @brief modelHandle縺ｮ繧ｲ繝・ち繝ｼ
	 */
	inline int GetModelHandle() const { return modelHandle; }

	/*
	 * @brief isActive縺ｮ繧ｻ繝・ち繝ｼ
	 */
	void SetActive(bool _isActive);

	/*
	 * @brief isActive縺ｮ繧ｲ繝・ち繝ｼ
	 */
	inline bool IsActive() const { return isActive; }

	/*
	 * @brief wantDelete縺ｮ繧ｻ繝・ち繝ｼ
	 */
	inline void SetWantDelete(bool _wantDelete) { wantDelete = _wantDelete; }

	/*
	 * @brief wantDelete縺ｮ繧ｲ繝・ち繝ｼ
	 */
	inline bool WantDelete() const { return wantDelete; }

	/*
	 * @brief pTransform縺ｮ繧ｲ繝・ち繝ｼ
	 */
	inline Transform* GetTransform() const { return pTransform; }

	/*
	 * @brief tag縺ｮ繧ｻ繝・ち繝ｼ
	 */
	inline void SetTag(Tag _tag) { tag = _tag; }

	/*
	 * @brief tag縺ｮ繧ｲ繝・ち繝ｼ
	 */
	inline Tag GetTag() const { return tag; }

	/*
	 * @brief 蠑墓焚縺ｮ繧ｿ繧ｰ縺瑚・霄ｫ縺ｨ蜷後§縺・
	 * @return 蠑墓焚縺瑚・霄ｫ縺ｮ繧ｿ繧ｰ縺ｨ荳閾ｴ縺励※繧九°
	 */
	bool CompareTag(Tag _tag) const;

	/*
	 * @brief 繧ｳ繝ｳ繝昴・繝阪Φ繝医・霑ｽ蜉
	 */
	template<typename T, typename... Args>
	T* AddComponent(Args&&... args);

	/*
	 * @brief 繧ｳ繝ｳ繝昴・繝阪Φ繝医・蜿門ｾ・
	 */
	template<typename T>
	T* GetComponent();

	inline std::string GetName() const { return name; }

	Vector3 GetFramePos(std::string _frameName);

	void ChangeMaterialColor(std::string _frameName, float _r,float _g,float _b);
};

template<typename T, typename ...Args>
inline T* GameObject::AddComponent(Args&&... args) {
	// 繧ｳ繝ｳ繝昴・繝阪Φ繝医〒辟｡縺代ｌ縺ｰ蟶ｰ繧・
	if (!std::is_base_of<ComponentBase, T>::value) return nullptr;
	// 逕滓・
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