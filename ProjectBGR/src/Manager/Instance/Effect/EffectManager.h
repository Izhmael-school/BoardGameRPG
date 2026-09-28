/*
 * @brief 繧ｨ繝輔ぉ繧ｯ繝医ｒ邂｡逅・☆繧九け繝ｩ繧ｹ
 * @author Sekino
 */
#pragma once

#ifndef _EFFECTMANAGER_H_
#define _EFFECTMANAGER_H_

#include "../../ManagerBase.h"
#include <memory>
#include <string>
#include <vector>
#include "Library/Vector/Vector3.h"
#include "Instance/EffectInstance.h"

class EffectInstance;
class EffectResourceManager;

class EffectManager : public ManagerBase {
private:
	std::vector<EffectPtr> instances;	// 邂｡逅・ｸ九↓縺ゅｋ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ
public:
	EffectResourceManager& pEffectResourceManager;// 隱ｭ縺ｿ霎ｼ縺ｿ逕ｨ

public:
	EffectManager(EffectResourceManager& _resourceManager);

	/*
	 * @brief 逕滓・
	 */
	EffectPtr Play(const std::string& _effectName, const Vector3& _pos, float _scale = 1.0f, const Vector3& _rot = VZero);

	/*
	 * @brief 譖ｴ譁ｰ
	 */
	void Update(float _t) override;

	/*
	 * @brief 謠冗判
	 */
	void Render() override;

	/*
	 * @brief 繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ縺ｮ蜈ｨ蜑企勁
	 */
	void Clean();

	/*
	 * @brief 邂｡逅・＠縺ｦ繧九う繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ縺ｮ謨ｰ
	 */
	int GetInstanceCount() const { return instances.size(); }
};
#endif