/*
 * @brief 繧ｨ繝輔ぉ繧ｯ繝医・繝ｪ繧ｽ繝ｼ繧ｹ繧堤ｮ｡逅・☆繧九け繝ｩ繧ｹ
 */
#pragma once
#ifndef _EFFECTRESOURCEMANAGER_H_
#define _EFFECTRESOURCEMANAGER_H_

#include <string>
#include <memory>
#include <unordered_map>

class EffectResource;

const char* const EFFECT_FILEPATH = "res/Effect/";	// 繧ｨ繝輔ぉ繧ｯ繝医・繝輔ぃ繧､繝ｫ繝代せ
const char* const EFFECTDATA_FILEPATH = "res/ExternalFile/Resource/EffectData.json";	// 繧ｨ繝輔ぉ繧ｯ繝医ョ繝ｼ繧ｿ縺ｮ繝輔ぃ繧､繝ｫ繝代せ

using EffectResourcePtr = std::shared_ptr<EffectResource>;

class EffectResourceManager {
private:
	std::unordered_map<std::string, EffectResourcePtr> resources;

public:
	EffectResourceManager();
	~EffectResourceManager() = default;

	/*
	 * @brief 繧ｨ繝輔ぉ繧ｯ繝医・隱ｭ縺ｿ霎ｼ縺ｿ
	 */
	bool LoadEffect(const std::string& _name, const std::string& _path);

	/*
	 * @brief 螟夜Κ繝輔ぃ繧､繝ｫ縺九ｉ縺ｮ繧ｨ繝輔ぉ繧ｯ繝郁ｪｭ縺ｿ霎ｼ縺ｿ
	 */
	void LoadEffectFromExternalFile();

	/*
	 * @brief 繧ｨ繝輔ぉ繧ｯ繝医Μ繧ｽ繝ｼ繧ｹ縺ｮ蜿門ｾ・
	 */
	EffectResourcePtr GetResource(const std::string& _name) const;

	/*
	 * @brief 隱ｭ縺ｿ霎ｼ繧薙□繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ謨ｰ蜿門ｾ・
	 */
	int GetEffectResourceCount() const { return resources.size(); }

	/*
	 * @brief 蜈ｨ繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ蜑企勁
	 */
	void Clear() { resources.clear(); }
};
#endif // !_EFFECTRESOURCEMANAGER_H_