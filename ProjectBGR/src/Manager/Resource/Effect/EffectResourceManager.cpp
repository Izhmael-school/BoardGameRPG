#include "EffectResourceManager.h"
#include "Resource/EffectResource.h"
#include "Definition/CommonModule/Json/MyJson.h"
#include <cassert>
#include "Definition/CommonModule/String/MyString.h"

EffectResourceManager::EffectResourceManager() {
}

bool EffectResourceManager::LoadEffect(const std::string& _name, const std::string& _path) {
	// 蜷悟錐縺ｮ逋ｻ骭ｲ遖∵ｭ｢
	if (!resources.empty())
		if (resources.contains(_name)) {
#if _DEBUG
			assert(false && "Effect Loaded");
#endif
			return false;
		}
	// 逕滓・
	auto resource = std::make_shared<EffectResource>(_name, _path);
	// 螟ｱ謨励＠縺溘ｉ蟶ｰ繧・
	if (!resource->Load()) return false;
	// 謌仙粥縺励◆繧蛾・蛻励↓
	resources.emplace(_name.c_str(), resource);
	return true;
}

void EffectResourceManager::LoadEffectFromExternalFile() {
	auto data = MyJson::LoadJsonFile(EFFECTDATA_FILEPATH);

	for (auto& d : data) {
		std::string name = d["name"];
		std::string fileName = d["path"];
		std::string path = MyString::MergeString(EFFECT_FILEPATH, fileName);

		LoadEffect(name, path);
	}
}

EffectResourcePtr EffectResourceManager::GetResource(const std::string& _name) const {
	auto itr = resources.find(_name);

	if (itr == resources.end()) {
#if _DEBUG
		assert(false && "Nothing Effect");
#endif 
		return nullptr;
	}

	return itr->second;
}