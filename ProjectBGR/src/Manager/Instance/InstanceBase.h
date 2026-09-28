/*
 * @brief 菴ｿ逕ｨ荳ｭ縺ｮ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧堤ｮ｡逅・☆繧句渕蠎輔け繝ｩ繧ｹ
 * @author Sekino
 */
#pragma once
#ifndef _INSTANCEBASE_H_
#define _INSTANCEBASE_H_

#include <memory>
#include "GameObject/GameObject.h"

class ResourceBase;

using ResourcePtr = std::shared_ptr<ResourceBase>;

class InstanceBase : public GameObject {
protected:
	ResourcePtr resource;	// 邏譚・
	bool wantDelete;	// 蜑企勁縺励※縺ｻ縺励＞縺・

public:
	InstanceBase(ResourcePtr _resource);
	~InstanceBase() = default;

	// 繧ｳ繝斐・遖∵ｭ｢
	InstanceBase(const InstanceBase&) = delete;
	InstanceBase& operator=(const InstanceBase&) = delete;
	// 遘ｻ蜍慕ｦ∵ｭ｢
	InstanceBase(InstanceBase&&) = delete;
	InstanceBase& operator=(InstanceBase&&) = delete;

	/*
	 * @brief 譖ｴ譁ｰ
	 */
	virtual void Update(float _t) override = 0;

	/*
	 * @brief 謠冗判
	 */
	virtual void Render() override = 0;

	/*
	 * @brief 蜑企勁隕∬ｫ・
	 */
	inline bool WantDelete() const { return wantDelete; }

	/*
	 * @brief 蜑企勁蜿ｯ蜷ｦ
	 */
	inline void SetDelete(bool _fact) { wantDelete = _fact; }
};
#endif