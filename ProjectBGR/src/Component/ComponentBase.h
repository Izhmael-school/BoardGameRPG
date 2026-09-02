/*
 * @brief コンポーネントの基底
 * @author Sekino
 */
#pragma once
#ifndef _COMPONENTBASE_H_
#define _COMPONENTBASE_H_

class GameObject;

class ComponentBase {
protected:
	GameObject* attachObject;	// コンポーネントがアタッチされたオブジェクト

	bool isActive;	// 有効か
public:
	ComponentBase(GameObject* _attachObject);
	virtual ~ComponentBase() = default;

	// コピー禁止
	ComponentBase(const ComponentBase&) = delete;
	ComponentBase& operator=(const ComponentBase&) = delete;

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
	 * @brief isActiveのセッター
	 */
	inline virtual void SetActive(bool _isActive) { isActive = _isActive; }

	/*
	 * @brief isActiveのゲッター
	 */
	inline virtual bool IsActive() const { return isActive; }

	/*
	 * @brief attachedObjectのゲッター
	 */
	inline GameObject* GetAttachObject() const { return attachObject; }
};

#endif