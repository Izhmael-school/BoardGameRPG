/*
 * @brief 管理クラスの基底
 * @author Sekino
 */
#pragma once
#ifndef _MANAGERBASE_H_
#define _MANAGERBASE_H_

class ManagerBase{
public:
	ManagerBase() = default;
	virtual ~ManagerBase() = default;

protected:
	/*
	 * @brief 生成時に入る（リソースロード等）
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
	 * @brief 後処理
	 */
	virtual void Cleanup();
};
#endif