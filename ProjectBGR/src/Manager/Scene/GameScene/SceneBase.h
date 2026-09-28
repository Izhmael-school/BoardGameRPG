/*
 * @brief 繧ｷ繝ｼ繝ｳ縺ｮ蝓ｺ蠎輔け繝ｩ繧ｹ
 * @author Sekino
 */
#pragma once

class UICanvasBase;
#include <memory>

class SceneBase {
protected:
	std::unique_ptr<UICanvasBase> sceneCanvas;

public:
	SceneBase();
	virtual ~SceneBase();

private:
	/*
	 * @brief 逕滓・譎ょ・逅・
	 */
	virtual void Start();

public:
	/*
	 * @brief 譖ｴ譁ｰ
	 */
	virtual void Update(float _t) = 0;
	/*
	 * @brief 謠冗判
	 */
	virtual void Render() = 0;
	/*
	 * @brief 蛻晄悄蛹・
	 */
	virtual void Setup();
	/*
	 * @brief 蠕悟・逅・
	 */
	virtual void Cleanup();

};