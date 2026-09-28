/*
 * @brief 繧ｨ繝輔ぉ繧ｯ繝医・繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧堤ｮ｡逅・☆繧九け繝ｩ繧ｹ
 * @author Sekino
 */
#pragma once
#ifndef _EFFECTINSTANCE_H_
#define _EFFECTINSTANCE_H_

#include "../../InstanceBase.h"
#include "Manager/Resource/Effect/Resource/EffectResource.h"
#include "Vector3.h"

class EffectResource;

class EffectInstance : public InstanceBase {
private:
	int playHandle;	// 繝ｪ繧ｽ繝ｼ繧ｹ縺梧戟縺｣縺ｦ縺・ｋ繝上Φ繝峨Ν
	
public:
	EffectInstance(std::shared_ptr<EffectResource> _effectResource);
	~EffectInstance() = default;

	/*
	 * @brief 譖ｴ譁ｰ
	 */
	void Update(float _t) override;

	/*
	 * @brief 謠冗判
	 */
	void Render() override;

	/*
	 * @brief 蜀咲函
	 */
	bool Play(Vector3 _pos,float _scale,Vector3 _rot = VZero);

	/*
	 * @brief 蛛懈ｭ｢
	 */
	void Stop();

	/*
	 * @brief 蜀咲函縺檎ｵゅｏ縺｣縺ｦ繧九°
	 */
	const bool IsEffectEnd() const;

private:
	void SetClassID();
};
#endif // !_EFFECTINSTANCE_H_

// 蛻･蜷・
using EffectPtr = std::shared_ptr<EffectInstance>;