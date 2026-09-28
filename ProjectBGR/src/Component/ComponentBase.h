/*
 * @brief 繧ｳ繝ｳ繝昴・繝阪Φ繝医・蝓ｺ蠎・
 * @author Sekino
 */
#pragma once
#ifndef _COMPONENTBASE_H_
#define _COMPONENTBASE_H_

class GameObject;

class ComponentBase {
protected:
	GameObject* attachObject;	// 繧ｳ繝ｳ繝昴・繝阪Φ繝医′繧｢繧ｿ繝・メ縺輔ｌ縺溘が繝悶ず繧ｧ繧ｯ繝・

	bool isActive;	// 譛牙柑縺・
public:
	ComponentBase(GameObject* _attachObject);
	virtual ~ComponentBase() = default;

	// 繧ｳ繝斐・遖∵ｭ｢
	ComponentBase(const ComponentBase&) = delete;
	ComponentBase& operator=(const ComponentBase&) = delete;

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
	 * @brief isActive縺ｮ繧ｻ繝・ち繝ｼ
	 */
	inline virtual void SetActive(bool _isActive) { isActive = _isActive; }

	/*
	 * @brief isActive縺ｮ繧ｲ繝・ち繝ｼ
	 */
	inline virtual bool IsActive() const { return isActive; }

	/*
	 * @brief attachedObject縺ｮ繧ｲ繝・ち繝ｼ
	 */
	inline GameObject* GetAttachObject() const { return attachObject; }
};

#endif