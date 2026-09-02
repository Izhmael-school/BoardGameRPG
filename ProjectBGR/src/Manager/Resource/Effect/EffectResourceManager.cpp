#include "EffectResourceManager.h"
#include "Resource/EffectResource.h"
#include "Definition/CommonModule/Json/MyJson.h"
#include <cassert>
#include "Definition/CommonModule/String/MyString.h"

bool EffectResourceManager::LoadEffect(const std::string& _name, const std::string& _path) {
	// 同名の登録禁止
	if (!resources.empty())
		if (resources.contains(_name)) {
#if _DEBUG
			assert(false && "Effect Loaded");
#endif
			return false;
		}
	// 生成
	auto resource = std::make_shared<EffectResource>(_name, _path);
	// 失敗したら帰る
	if (!resource->Load()) return false;
	// 成功したら配列に
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