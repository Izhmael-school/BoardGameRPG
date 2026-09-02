/*
 * @brief シーンの基底クラス
 * @author Sekino
 */
#pragma once

class SceneBase {
public:
	SceneBase();
	virtual ~SceneBase() = default;

private:
	/*
	 * @brief 生成時処理
	 */
	virtual void Start();

public:
	/*
	 * @brief 更新
	 */
	virtual void Update(float _t) = 0;
	/*
	 * @brief 描画
	 */
	virtual void Render() = 0;
	/*
	 * @brief 初期化
	 */
	virtual void Setup();
	/*
	 * @brief 後処理
	 */
	virtual void Cleanup();

};