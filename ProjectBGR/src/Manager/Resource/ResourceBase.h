/*
 * @brief DX繝ｩ繧､繝悶Λ繝ｪ繧剃ｽｿ縺｣縺ｦ隱ｭ縺ｿ霎ｼ繧繝ｪ繧ｽ繝ｼ繧ｹ蝓ｺ蠎輔け繝ｩ繧ｹ
 * @brief 1繝輔ぃ繧､繝ｫ縺ｫ縺､縺・繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ
 * @author Sekino
 */

#pragma once
#ifndef _RESOURCEBASE_H_
#define _RESOURCEBASE_H_

#include <string>

class ResourceBase {
protected:
	// 蜷榊燕
	std::string name;
	// 繝輔ぃ繧､繝ｫ繝代せ
	std::string path;
	// 隱ｭ縺ｿ霎ｼ繧薙□繝上Φ繝峨Ν
	int loadHandle;

public:
	ResourceBase(const std::string& _name, const std::string& _path);
	virtual ~ResourceBase() = default;

	/*
	 * @brief 隱ｭ縺ｿ霎ｼ縺ｿ
	 */
	virtual bool Load() = 0;

	/*
	 * @brief 隱ｭ縺ｿ霎ｼ繧√◆縺・
	 */
	bool IsLoaded() const { return loadHandle != -1; }

	/*
	 * @brief 繝上Φ繝峨Ν縺ｮ蜿門ｾ・
	 */
	int GetHandle() const { return loadHandle; }

	const std::string GetName() const { return name; }
	const std::string GetPath() const { return path; }

public:
	// 繧ｳ繝斐・遖∵ｭ｢
	ResourceBase(const ResourceBase&) = delete;
	ResourceBase& operator = (const ResourceBase&) = delete;
	// 遘ｻ蜍慕ｦ∵ｭ｢
	ResourceBase(ResourceBase&&) = delete;
	ResourceBase& operator = (ResourceBase&&) = delete;

};
#endif // !_RESOURCEBASE_H_