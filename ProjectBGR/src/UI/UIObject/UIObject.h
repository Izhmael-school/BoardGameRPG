/*
 * @brief UIの基底
 * @author Sekino
 */

#pragma once
#ifndef _UIOBJECT_H_
#define _UIOBJECT_H_

#include "Vector2.h"
#include <vector>
#include <memory>

class UIInput;

class UIObject {
protected:
	// 座標
	Vector2 position;
	// 表示フラグ
	bool isActive;
	// 親オブジェクト
	UIObject* parent;
	// 子オブジェクト
	std::vector<std::unique_ptr<UIObject>> children;

public:
	UIObject();
	virtual ~UIObject();

	/*
	 * @brief 初期設定
	 */
	void Init();

	/*
	 * @brief 更新処理
	 */
	void Update(float _t);

	/*
	 * @brief 更新処理
	 */
	void Update(float _t, UIInput& _input);

	/*
	 * @brief 描画処理
	 */
	void Render();

	/*
	 * @brief 終了処理
	 */
	void End();

protected:
	/*
	 * @brief Initで個別で処理したいことがあればoverrideする
	 */
	virtual void OnInit() {};

	/*
	 * @brief Updataで個別で処理したいことがあればoverrideする
	 */
	virtual void OnUpdate(float _t) {};

	virtual void OnUpdate(float _t, UIInput& _input) {};
	/*
	 * @brief Renderで個別で処理したいことがあればoverrideする
	 */
	virtual void OnRender() {};

	/*
	 * @brief Endで個別で処理したいことがあればoverrideする
	 */
	virtual void OnEnd() {};

public:
	/*
	 * @brief 座標を設定
	 */
	inline void SetPosition(Vector2 _pos) { position = _pos; }
	inline void SetPosition(float _x,float _y) { position = Vector2(_x,_y); }

	/*
	 * @brief 座標を取得
	 */
	inline Vector2 GetPosition() const { return position; }

	/*
	 * @brief ワールド座標の取得
	 */
	Vector2 GetWorldPosition() const;

	/*
	 * @brief 表示フラグを設定
	 */
	inline void SetActive(bool _isActive) { isActive = _isActive; }

	/*
	 * @brief 表示フラグの取得
	 */
	inline bool IsActive() const { return isActive; }

	/*
	 * @brief 親の表示フラグを考慮したフラグを取得
	 */
	bool IsActiveInParent();

	/*
	 * @brief 子の追加
	 */
	void AddChild(std::unique_ptr<UIObject> _child);

	/*
	 * @brief 子のリスト取得
	 */
	std::vector<UIObject*> GetChildren() const;

	/*
	 * @brief 親の取得
	 */
	inline UIObject* GetParent() const { return parent; }
};

#endif // !_UIOBJECT_H_